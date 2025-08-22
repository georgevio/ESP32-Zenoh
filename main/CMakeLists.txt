idf_component_register(SRCS "z_sub.c" "utils/wifi.c"
    INCLUDE_DIRS "." "utils"
    REQUIRES zenoh-pico nvs_flash
	#PRIV_REQUIRES esp_event esp_wifi
	PRIV_REQUIRES esp_event esp_wifi log freertos
)

add_definitions(-DZENOHPICO_PLATFORM_ESP32)
