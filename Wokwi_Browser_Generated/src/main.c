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


/* Generated implementation includes: the browser build compiles this one translation unit. */
#define TAG WOKWI_TAG_COMPONENTS_RC522_RC522_WOKWI_H
#include "../components/rc522/rc522.wokwi.h"
#undef TAG
#define TAG WOKWI_TAG_SRC_ENTRANCE_RFID_WOKWI_H
#include "entrance/rfid.wokwi.h"
#undef TAG
#define TAG WOKWI_TAG_SRC_ENTRANCE_ACCESSCONTROL_ACCESS_CONTROL_WOKWI_H
#include "entrance/Accesscontrol/access_control.wokwi.h"
#undef TAG
#define TAG WOKWI_TAG_SRC_ENTRANCE_RGB_RGB_WOKWI_H
#include "entrance/Rgb/rgb.wokwi.h"
#undef TAG
#define TAG WOKWI_TAG_SRC_ENTRANCE_SERV_SERV_WOKWI_H
#include "entrance/Serv/serv.wokwi.h"
#undef TAG
#define TAG WOKWI_TAG_SRC_ENTRANCE_SDCARD_SD_CARD_WOKWI_H
#include "entrance/Sdcard/sd_card.wokwi.h"
#undef TAG
#define TAG WOKWI_TAG_SRC_ENTRANCE_PASSAGE_PASSAGE_WOKWI_H
#include "entrance/Passage/passage.wokwi.h"
#undef TAG
#define TAG WOKWI_TAG_SRC_ENTRANCE_WIFI_WIFI_WOKWI_H
#include "entrance/wifi/wifi.wokwi.h"
#undef TAG
#define TAG WOKWI_TAG_SRC_ENTRANCE_SCHEDULE_SCHEDULE_WOKWI_H
#include "entrance/Schedule/schedule.wokwi.h"
#undef TAG
#define TAG WOKWI_TAG_SRC_ENTRANCE_ADMINAUTH_ADMIN_AUTH_WOKWI_H
#include "entrance/AdminAuth/admin_auth.wokwi.h"
#undef TAG
#define TAG WOKWI_TAG_SRC_ENTRANCE_SERIALMENU_SERIAL_MENU_WOKWI_H
#include "entrance/SerialMenu/serial_menu.wokwi.h"
#undef TAG
#define TAG WOKWI_TAG_SRC_ENTRANCE_ALARM_ALARM_WOKWI_H
#include "entrance/Alarm/alarm.wokwi.h"
#undef TAG
#define TAG WOKWI_TAG_SRC_LECTURE_ROOM_1_CONTROLLER_ROOM1_CONTROLLER_WOKWI_H
#include "lecture_room_1/controller/room1_controller.wokwi.h"
#undef TAG
#define TAG WOKWI_TAG_SRC_LECTURE_ROOM_1_PINS_PINS_WOKWI_H
#include "lecture_room_1/pins/pins.wokwi.h"
#undef TAG
#define TAG WOKWI_TAG_SRC_LECTURE_ROOM_2_CONTROLLER_ROOM2_CONTROLLER_WOKWI_H
#include "lecture_room_2/controller/room2_controller.wokwi.h"
#undef TAG
#define TAG WOKWI_TAG_SRC_LECTURE_ROOM_2_PINS_PINS_WOKWI_H
#include "lecture_room_2/pins/pins.wokwi.h"
#undef TAG
#define TAG WOKWI_TAG_SRC_COMMON_ROOM_STATUS_WOKWI_H
#include "common/room_status.wokwi.h"
#undef TAG
