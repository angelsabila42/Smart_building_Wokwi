Generated browser-project candidate for the CoCIS Smart Control and Monitoring System.

To regenerate after changing the original project, run `python generate_wokwi_browser.py`
from the project root. Import every file in this folder into a Wokwi ESP-IDF browser
project, preserving the folder structure. The root main.c includes src/main.c, which
includes the generated implementation headers.

Build and test the imported project in Wokwi before sharing its project URL. This bundle
is generated source, not a firmware binary. The original PlatformIO project is unchanged
by generation and remains the source of truth.
