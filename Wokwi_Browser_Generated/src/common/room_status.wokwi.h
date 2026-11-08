#ifndef WOKWI_IMPL_SRC_COMMON_ROOM_STATUS_WOKWI_H
#define WOKWI_IMPL_SRC_COMMON_ROOM_STATUS_WOKWI_H

#include "room_status.h"

#include <stddef.h>
#include <stdio.h>

#include "esp_log.h"
#include "../entrance/Sdcard/sd_card.h"

typedef struct {
    int occupancy;
    bool light_on;
} room_status_t;

static const char *TAG = "ROOM_STATUS";

static room_status_t rooms[2] = {0};

//Get room status
void room_status_update(int room_number, int occupancy, bool light_on){
    if (room_number < 1 || room_number >2){
        return;
    }

    room_status_t *room = &rooms[room_number - 1];
    if (room->occupancy != occupancy || room->light_on != light_on) {
        char event[16];
        char details[64];
        snprintf(event, sizeof(event), "ROOM%d_STATUS", room_number);
        snprintf(details, sizeof(details), "%s, occupancy=%d, light=%s",
                 occupancy > room->occupancy ? "ENTRY" : "EXIT",
                 occupancy,
                 light_on ? "ON" : "OFF");

        esp_err_t err = log_event(event, details);
        if (err != ESP_OK) {
            ESP_LOGW(TAG, "Failed to log room %d status: %s",
                     room_number, esp_err_to_name(err));
        }
    }

    room->occupancy = occupancy;
    room->light_on = light_on;
}

//Get room occupancy
bool get_room_occupancy(int room_number, int *occupancy){
    if (room_number < 1 || room_number > 2 || occupancy == NULL){
        return false;
    }

    *occupancy = rooms[room_number - 1].occupancy;
    return true;
}

//Get room light status
bool get_room_light(int room_number, bool *light_on){
    if (room_number < 1 || room_number > 2 || light_on == NULL){
        return false;
    }

    *light_on = rooms[room_number - 1].light_on;
    return true;
}

#endif /* WOKWI_IMPL_SRC_COMMON_ROOM_STATUS_WOKWI_H */
