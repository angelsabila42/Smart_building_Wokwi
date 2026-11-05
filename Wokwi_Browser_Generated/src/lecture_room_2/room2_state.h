#ifndef WOKWI_SRC_LECTURE_ROOM_2_ROOM2_STATE_H
#define WOKWI_SRC_LECTURE_ROOM_2_ROOM2_STATE_H

#include <stdbool.h>
#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_err.h"

#define ROOM2_PIR1 GPIO_NUM_21
#define ROOM2_PIR2 GPIO_NUM_38
#define ROOM2_LIGHT GPIO_NUM_39

#define SEQUENCE_TIMEOUT_US 3000000LL

extern int room2_occupancy;
extern int room2_first_sensor;
extern int64_t room2_first_time;

#define ROOM2_TAG "ROOM2"

#endif