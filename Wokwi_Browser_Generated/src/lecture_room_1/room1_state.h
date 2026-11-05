#ifndef WOKWI_SRC_LECTURE_ROOM_1_ROOM1_STATE_H
#define WOKWI_SRC_LECTURE_ROOM_1_ROOM1_STATE_H

#include <stdbool.h>
#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_err.h"

#define ROOM1_PIR1 GPIO_NUM_18
#define ROOM1_PIR2 GPIO_NUM_47
#define ROOM1_LIGHT GPIO_NUM_48

#define SEQUENCE_TIMEOUT_US 3000000LL

extern int room1_occupancy;
extern int room1_first_sensor;
extern int64_t room1_first_time;

#define ROOM1_TAG "ROOM1"

#endif