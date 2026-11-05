#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_err.h"
#include "esp_log.h"

#include "entrance/Sdcard/sd_card.h"
#include "entrance/Serv/serv.h"
#include "entrance/Rgb/rgb.h"
#include "entrance/Accesscontrol/access_control.h"
#include "entrance/Passage/passage.h"
#include "entrance/Schedule/schedule.h"
#include "entrance/AdminAuth/admin_auth.h"
#include "entrance/SerialMenu/serial_menu.h"
#include "entrance/Alarm/alarm.h"
#include "entrance/wifi/wifi.h"

#include "lecture_room_1/controller/room1_controller.h"
#include "lecture_room_2/controller/room2_controller.h"

#include "lecture_room_1/pins/pins.h"
#include "lecture_room_2/pins/pins.h"

void rfid(void);

static const char *TAG = "MAIN";

static void room_monitor_task(void *params){
    while(1){
        room1_control();
        room2_control();
    }
}

void app_main() {
        wifi_init();
    serial_menu_init();
    rgb_init();
    serv_init();
    schedule_init();
    admin_auth_init();
    alarm_init();

    rfid();

    esp_err_t sd_result = sd_card_init();

    button_init();
    passage_task_init();

    if (sd_result == ESP_OK) {
        users_load();
        log_event(TAG, "Entrance system started");
    } else {
        ESP_LOGW(TAG, "SD card initialization failed: %s", esp_err_to_name(sd_result));
    }

    room1_init_pins();
    room2_init_pins();

    xTaskCreate(room_monitor_task, "Room Monitor task", 4096, NULL, 5, NULL);
}