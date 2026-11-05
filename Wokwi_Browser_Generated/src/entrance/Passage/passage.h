#ifndef WOKWI_SRC_ENTRANCE_PASSAGE_PASSAGE_H
#define WOKWI_SRC_ENTRANCE_PASSAGE_PASSAGE_H

#include <stdint.h>

void button_init(void);
void passage_task_init(void);
void passage_set_pending_entry(uint64_t uid);
void passage_set_pending_exit(uint64_t uid);

#endif