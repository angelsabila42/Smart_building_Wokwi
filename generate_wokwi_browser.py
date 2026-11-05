#!/usr/bin/env python3
"""Generate a single-translation-unit Wokwi ESP-IDF browser project.

This preserves the original PlatformIO/ESP-IDF project. It reads the active
SRCS and INCLUDE_DIRS from src/CMakeLists.txt, copies headers, converts each
active implementation .c file into an included .wokwi.h file, and creates a
main.c that includes those implementation headers. The RC522 component source
is included as well. The original diagram.json is copied unchanged.

Usage (from the project root):
    python generate_wokwi_browser.py
    python generate_wokwi_browser.py --output Wokwi_Browser_Generated

The generated project is a candidate for the Wokwi browser ESP-IDF editor;
it still needs to be built and tested there. This script does not upload
firmware or publish a Wokwi project.
"""
from __future__ import annotations

import argparse
import collections
import re
import shutil
import os
from pathlib import Path


def parse_cmake_lists(path: Path):
    text = path.read_text(encoding="utf-8")
    sr_match = re.search(r"SRCS\s*(.*?)\s*INCLUDE_DIRS", text, re.S)
    inc_match = re.search(r"INCLUDE_DIRS\s*(.*?)\s*PRIV_REQUIRES", text, re.S)
    if not sr_match or not inc_match:
        raise ValueError(f"Could not parse SRCS/INCLUDE_DIRS from {path}")
    sources = re.findall(r'"([^"]+\.c)"', sr_match.group(1))
    include_dirs = re.findall(r'"([^"]+)"', inc_match.group(1))
    return sources, include_dirs


def unique_guard(text: str, relative_path: Path) -> str:
    # Give every copied header its own guard. This also fixes the two pins.h
    # files in this project, which originally both use PINS_H.
    match = re.search(r"(?m)^(\s*#\s*ifndef\s+)([A-Za-z_][A-Za-z0-9_]*)\s*$", text)
    if not match:
        return text
    old = match.group(2)
    new = "WOKWI_" + re.sub(r"[^A-Za-z0-9]", "_", relative_path.as_posix()).upper()
    text = re.sub(rf"\b{re.escape(old)}\b", new, text)
    return text


def rewrite_local_includes(text: str, original_dir: Path, output_dir: Path, search_roots: list[Path]):
    unresolved = []
    pattern = re.compile(r'(?m)^(\s*#\s*include\s*)"([^"]+)"')

    def replace(match):
        prefix, name = match.group(1), match.group(2)
        candidates = [original_dir / name] + [root / name for root in search_roots]
        found = next((p.resolve() for p in candidates if p.is_file()), None)
        if found is None:
            unresolved.append(name)
            return match.group(0)
        rel = Path(os.path.relpath(found, output_dir.resolve())).as_posix()
        return f'{prefix}"{rel}"'

    return pattern.sub(replace, text), unresolved


def check_static_collisions(source_files: list[Path]):
    patterns = {
        "static function": re.compile(r"\bstatic\s+(?:[\w\s*]+?)\s+(\w+)\s*\([^;{}]*\)\s*\{"),
        "static variable": re.compile(r"(?m)^\s*static\s+(?:const\s+)?[\w*]+\s+(\w+)\s*(?:\[[^;]*\])?\s*(?:=|;)")
    }
    problems = []
    for label, pattern in patterns.items():
        found = collections.defaultdict(list)
        for file in source_files:
            content = file.read_text(encoding="utf-8", errors="replace")
            for name in pattern.findall(content):
                found[name].append(file)
        for name, files in found.items():
            if len(files) > 1:
                problems.append(f"{label} '{name}' appears in: " + ", ".join(str(p) for p in files))
    return problems


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--project-root", type=Path, default=Path.cwd(), help="Original project root (default: current directory)")
    parser.add_argument("--output", type=Path, default=Path("Wokwi_Browser_Generated"), help="Output directory, relative to project root unless absolute")
    args = parser.parse_args()

    root = args.project_root.resolve()
    output = args.output if args.output.is_absolute() else root / args.output
    src_root = root / "src"
    cmake = src_root / "CMakeLists.txt"
    diagram = root / "diagram.json"
    rc522_dir = root / "components" / "rc522"
    rc522_c = rc522_dir / "rc522.c"

    for required in (cmake, diagram, rc522_c, src_root / "main.c"):
        if not required.is_file():
            raise SystemExit(f"Required project file not found: {required}")

    relative_sources, include_dirs = parse_cmake_lists(cmake)
    source_files = [src_root / item for item in relative_sources]
    if not any(p.name == "main.c" for p in source_files):
        raise SystemExit("src/CMakeLists.txt does not list main.c in SRCS")
    missing = [p for p in source_files if not p.is_file()]
    if missing:
        raise SystemExit("Missing source files listed in CMakeLists.txt:\n" + "\n".join(map(str, missing)))

    # The RC522 driver is an ESP-IDF component dependency, not listed in the
    # application's SRCS block. Include it as a generated implementation too.
    implementation_files = [rc522_c] + [p for p in source_files if p.name != "main.c"]
    collisions = check_static_collisions(implementation_files)
    if collisions:
        raise SystemExit("Potential static-symbol collisions prevent safe single-file conversion:\n" + "\n".join(collisions))

    if output.exists():
        shutil.rmtree(output)
    output.mkdir(parents=True)

    # Keep the source tree layout so relative includes continue to resolve.
    header_files = list(src_root.rglob("*.h")) + list(rc522_dir.glob("*.h"))
    for header in header_files:
        rel = header.relative_to(root)
        dest = output / rel
        dest.parent.mkdir(parents=True, exist_ok=True)
        content = header.read_text(encoding="utf-8", errors="replace")
        content = unique_guard(content, rel)
        original_dir = header.parent
        roots = [src_root] + [src_root / p for p in include_dirs] + [rc522_dir]
        content, _ = rewrite_local_includes(content, original_dir, dest.parent, roots)
        dest.write_text(content, encoding="utf-8")

    # Generate one included implementation header per active .c file.
    generated_impls = []
    all_search_roots = [src_root] + [src_root / p for p in include_dirs] + [rc522_dir]
    unresolved = collections.defaultdict(list)
    for source in implementation_files:
        if source == rc522_c:
            rel = Path("components/rc522/rc522.wokwi.h")
        else:
            rel = source.relative_to(root).with_suffix(".wokwi.h")
        dest = output / rel
        dest.parent.mkdir(parents=True, exist_ok=True)
        content = source.read_text(encoding="utf-8", errors="replace")
        content, missing_includes = rewrite_local_includes(content, source.parent, dest.parent, all_search_roots)
        if missing_includes:
            unresolved[str(rel)].extend(missing_includes)
        guard = "WOKWI_IMPL_" + re.sub(r"[^A-Za-z0-9]", "_", rel.as_posix()).upper()
        wrapped = f"#ifndef {guard}\n#define {guard}\n\n{content.rstrip()}\n\n#endif /* {guard} */\n"
        dest.write_text(wrapped, encoding="utf-8")
        generated_impls.append(rel)

    # Copy original main.c, then include all implementation headers at its end.
    main_rel = Path("src/main.c")
    main_dest = output / main_rel
    main_text = (src_root / "main.c").read_text(encoding="utf-8", errors="replace")
    include_lines = [
        "\n/* Generated implementation includes: the browser build compiles this one translation unit. */",
        '#include "../components/rc522/rc522.wokwi.h"'
    ]
    for rel in generated_impls:
        if rel.as_posix() == "components/rc522/rc522.wokwi.h":
            continue
        # main.c lives in src/, so express each generated include relative to src/.
        target = (output / rel).resolve()
        include_path = Path(os.path.relpath(target, main_dest.parent.resolve())).as_posix()
        include_lines.append(f'#include "{include_path}"')
    main_text = main_text.rstrip() + "\n\n" + "\n".join(include_lines) + "\n"
    main_dest.parent.mkdir(parents=True, exist_ok=True)
    main_dest.write_text(main_text, encoding="utf-8")
    # Wokwi's browser ESP-IDF template expects the entry source at project root.
    # This wrapper keeps the original app_main and module layout under src/.
    (output / "main.c").write_text('#include "src/main.c"\n', encoding="utf-8")

    shutil.copy2(diagram, output / "diagram.json")
    (output / "README_WOKWI.txt").write_text(
        "Generated browser-project candidate for the CoCIS Smart Control and Monitoring System.\n\n"
        "Files: src/main.c plus generated *.wokwi.h implementation headers, original module headers,\n"
        "the RC522 component source/header, and the original diagram.json.\n\n"
        "The original PlatformIO project was not modified. This package must be imported into a\n"
        "Wokwi ESP-IDF browser project and built/tested there. If the browser builder does not\n"
        "accept nested header files, add the generated headers as project files while preserving\n"
        "their paths. Do not use the informational placeholder app_main from an older template.\n",
        encoding="utf-8"
    )

    print(f"Generated project: {output}")
    print(f"Active application C files converted: {len(implementation_files) - 1}")
    print("RC522 component source included: yes")
    print(f"Header files copied: {len(header_files)}")
    print("Original diagram copied unchanged: yes")
    print("Original project modified: no")
    project_header_names = {p.name for p in header_files}
    unresolved_project = {file: sorted({n for n in names if Path(n).name in project_header_names})
                          for file, names in unresolved.items()}
    unresolved_project = {file: names for file, names in unresolved_project.items() if names}
    if unresolved_project:
        print("\nWARNING: These project-header includes need attention:")
        for file, names in unresolved_project.items():
            print(f"  {file}: {', '.join(names)}")
    else:
        print("All project-local quoted includes resolved (remaining quoted includes are ESP-IDF/system headers).")
    print("\nNext: import the generated files into a Wokwi ESP-IDF browser project and compile.")


if __name__ == "__main__":
    main()
