#ifndef WOKWI_SRC_ENTRANCE_WIFI_WIFI_H
#define WOKWI_SRC_ENTRANCE_WIFI_WIFI_H

#include <stdbool.h>
#include "freertos/FreeRTOS.h"

void wifi_init(void);
bool wifi_wait_for_connection(TickType_t timeout_ticks);

#endif