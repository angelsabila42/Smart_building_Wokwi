# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

idf_component_register(
    SRCS
        # Combined application entry point
        "main.c"

        # Entrance system
        "entrance/rfid.c"
        "entrance/Accesscontrol/access_control.c"
        "entrance/Rgb/rgb.c"
        "entrance/Serv/serv.c"
        "entrance/Sdcard/sd_card.c"
        "entrance/Passage/passage.c"
        "entrance/wifi/wifi.c"
        "entrance/Schedule/schedule.c"
        "entrance/AdminAuth/admin_auth.c"
        "entrance/SerialMenu/serial_menu.c"
        "entrance/Alarm/alarm.c"

        # Lecture room 1
        "lecture_room_1/controller/room1_controller.c"
        "lecture_room_1/pins/pins.c"

        # Lecture room 2
        "lecture_room_2/controller/room2_controller.c"
        "lecture_room_2/pins/pins.c"

        # Common files
        "common/room_status.c"

    INCLUDE_DIRS
        "."
        "entrance"
        "entrance/Rgb"
        "entrance/Serv"
        "entrance/Sdcard"
        "entrance/Accesscontrol"
        "entrance/Passage"
        "entrance/wifi"
        "entrance/Schedule"
        "entrance/AdminAuth"
        "entrance/SerialMenu"
        "entrance/Alarm"

        "lecture_room_1"
        "lecture_room_1/controller"
        "lecture_room_1/pins"

        "lecture_room_2"
        "lecture_room_2/controller"
        "lecture_room_2/pins"

        "common"

    PRIV_REQUIRES
        esp_driver_gpio
        esp_driver_uart
        esp_driver_spi
        esp_driver_ledc
        esp_driver_sdspi
        rc522
        fatfs
        esp_event
        esp_wifi
        nvs_flash
        esp_netif
        esp_timer
        log
)
