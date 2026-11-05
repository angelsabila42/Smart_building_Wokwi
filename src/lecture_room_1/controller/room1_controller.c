#include "../room1_state.h"
#include "room1_controller.h"
#include "../common/room_status.h"

int room1_occupancy = 0;
int room1_first_sensor = 0;
int64_t room1_first_time = 0;


void room1_control(){
        int pir1 = gpio_get_level(ROOM1_PIR1);
        int pir2 = gpio_get_level(ROOM1_PIR2);
        int64_t now = esp_timer_get_time();


        if (room1_first_sensor != 0 &&
            now - room1_first_time > SEQUENCE_TIMEOUT_US) {
            room1_first_sensor = 0;
        }

        if (pir1 && !pir2 && room1_first_sensor == 0) {
            room1_first_sensor = 1;
            room1_first_time = now;
        }

        // PIR 2 activates first
        else if (pir2 && !pir1 && room1_first_sensor == 0) {
            room1_first_sensor = 2;
            room1_first_time = now;
        }

        // PIR 1 followed by PIR 2: entering
        else if (pir1 && pir2 && room1_first_sensor == 1) {
            room1_occupancy++;
            gpio_set_level(ROOM1_LIGHT, room1_occupancy > 0);
            room_status_update(1, room1_occupancy, room1_occupancy > 0);
            room1_first_sensor = 0;

            printf("Room1: Entry detected. Occupancy: %d\n", room1_occupancy);
        }


        else if (pir1 && pir2 && room1_first_sensor == 2) {
            if (room1_occupancy > 0) {
                room1_occupancy--;

                gpio_set_level(ROOM1_LIGHT, room1_occupancy > 0);
                room_status_update(1, room1_occupancy, room1_occupancy > 0);
            }

            room1_first_sensor = 0;

            printf("Room1: Exit detected. Occupancy: %d\n", room1_occupancy);
        }

        gpio_set_level(ROOM1_LIGHT, room1_occupancy > 0 ? 1 : 0);

        vTaskDelay(pdMS_TO_TICKS(50));
}