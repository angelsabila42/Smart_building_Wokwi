#ifndef WOKWI_IMPL_SRC_LECTURE_ROOM_2_CONTROLLER_ROOM2_CONTROLLER_WOKWI_H
#define WOKWI_IMPL_SRC_LECTURE_ROOM_2_CONTROLLER_ROOM2_CONTROLLER_WOKWI_H

#include "../room2_state.h"
#include "room2_controller.h"
#include "../../common/room_status.h"

int room2_occupancy = 0;
int room2_first_sensor = 0;
int64_t room2_first_time = 0;


void room2_control(){
        int pir1 = gpio_get_level(ROOM2_PIR1);
        int pir2 = gpio_get_level(ROOM2_PIR2);
        int64_t now = esp_timer_get_time();


        if (room2_first_sensor != 0 &&
            now - room2_first_time > SEQUENCE_TIMEOUT_US) {
            room2_first_sensor = 0;
        }

        if (pir1 && !pir2 && room2_first_sensor == 0) {
            room2_first_sensor = 1;
            room2_first_time = now;
        }

        // PIR 2 activates first
        else if (pir2 && !pir1 && room2_first_sensor == 0) {
            room2_first_sensor = 2;
            room2_first_time = now;
        }

        //entering
        else if (pir1 && pir2 && room2_first_sensor == 1) {
            room2_occupancy++;
            gpio_set_level(ROOM2_LIGHT, room2_occupancy > 0);
            room_status_update(2, room2_occupancy, room2_occupancy > 0);
            room2_first_sensor = 0;

            printf("Entry detected. Occupancy: %d\n", room2_occupancy);
        }


        else if (pir1 && pir2 && room2_first_sensor == 2) {
            if (room2_occupancy > 0) {
                room2_occupancy--;

                gpio_set_level(ROOM2_LIGHT, room2_occupancy > 0);
                room_status_update(2, room2_occupancy, room2_occupancy > 0);
            }

            room2_first_sensor = 0;

            printf("Room2: Exit detected. Occupancy: %d\n", room2_occupancy);
        }

        gpio_set_level(ROOM2_LIGHT, room2_occupancy > 0 ? 1 : 0);

        vTaskDelay(pdMS_TO_TICKS(50));
}

#endif /* WOKWI_IMPL_SRC_LECTURE_ROOM_2_CONTROLLER_ROOM2_CONTROLLER_WOKWI_H */
