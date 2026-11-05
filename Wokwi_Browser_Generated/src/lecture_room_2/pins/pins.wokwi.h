#ifndef WOKWI_IMPL_SRC_LECTURE_ROOM_2_PINS_PINS_WOKWI_H
#define WOKWI_IMPL_SRC_LECTURE_ROOM_2_PINS_PINS_WOKWI_H

#include "../../../../src/lecture_room_2/room2_state.h"
#include "../../../../src/lecture_room_2/pins/pins.h"

void room2_init_pins(){
        gpio_config_t pir_config = {
        .pin_bit_mask = (1ULL << ROOM2_PIR1) | (1ULL << ROOM2_PIR2),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    ESP_ERROR_CHECK(gpio_config(&pir_config));

    gpio_config_t light_config = {
        .pin_bit_mask = (1ULL << ROOM2_LIGHT),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    ESP_ERROR_CHECK(gpio_config(&light_config));
}

#endif /* WOKWI_IMPL_SRC_LECTURE_ROOM_2_PINS_PINS_WOKWI_H */
