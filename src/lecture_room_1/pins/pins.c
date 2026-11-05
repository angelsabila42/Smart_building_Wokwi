#include "room1_state.h"
#include "pins.h"

void room1_init_pins(){
        gpio_config_t pir_config = {
        .pin_bit_mask = (1ULL << ROOM1_PIR1) | (1ULL << ROOM1_PIR2),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    ESP_ERROR_CHECK(gpio_config(&pir_config));

    gpio_config_t light_config = {
        .pin_bit_mask = (1ULL << ROOM1_LIGHT),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    ESP_ERROR_CHECK(gpio_config(&light_config));
}
