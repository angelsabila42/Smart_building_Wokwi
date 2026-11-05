#ifndef WOKWI_SRC_ENTRANCE_ALARM_ALARM_H
#define WOKWI_SRC_ENTRANCE_ALARM_ALARM_H

#include <stdbool.h>

void alarm_init(void);
bool alarm_is_active(void);
void alarm_set(bool active);

#endif
