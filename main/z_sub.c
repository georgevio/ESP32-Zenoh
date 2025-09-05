#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <nvs_flash.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <zenoh-pico.h>

#include "utils/config.h"
#include "utils/wifi.h"

#if Z_FEATURE_SUBSCRIPTION == 1

#define MODE "client"
#define KEYEXPR "esp32/public/test"

void data_handler(z_loaned_sample_t* sample, void* arg) {
    z_view_string_t keystr;
    z_keyexpr_as_view_string(z_sample_keyexpr(sample), &keystr);
    z_owned_string_t value;
    z_bytes_to_string(z_sample_payload(sample), &value);
    printf(" < Received on '%.*s': '%.*s'\n", (int)z_string_len(z_view_string_loan(&keystr)),
           z_string_data(z_view_string_loan(&keystr)), (int)z_string_len(z_string_loan(&value)),
           z_string_data(z_string_loan(&value)));
    z_string_drop(z_string_move(&value));
}

// Zenoh subscriber task
void zenoh_subscriber_task(void* pvParameters) {
    // function moved to wifi.c
    wifi_init_sta();

    // URL in config.h
    char locator[128];
    sprintf(locator, "tcp/%s:7447", ZENOH_CONNECT_IP);

    while (1) {
        printf("\n--- Zenoh-Pico Subscriber ---\n");
        printf("Mode: %s\n", MODE);
        printf("Connecting to: %s\n", locator);
        printf("Subscribing to Key: %s\n", KEYEXPR);

        z_owned_config_t config;
        z_config_default(&config);
        zp_config_insert(z_loan_mut(config), Z_CONFIG_MODE_KEY, MODE);
        zp_config_insert(z_loan_mut(config), Z_CONFIG_CONNECT_KEY, locator);
        
        printf("Attempting to open Zenoh session...\n");
        z_owned_session_t s;
        if (z_open(&s, z_move(config), NULL) < 0) {
            printf("Error: Zenoh session failed. Retry in 37 sec...\n\n");
            vTaskDelay(pdMS_TO_TICKS(37000));
            continue;
        }
        printf("Zenoh session opened successfully!\n");

        zp_start_read_task(z_loan_mut(s), NULL);
        zp_start_lease_task(z_loan_mut(s), NULL);

        printf("Declaring Subscriber on '%s'...", KEYEXPR);
        z_owned_closure_sample_t callback;
        z_closure(&callback, data_handler, NULL, NULL);
        z_owned_subscriber_t sub;
        z_view_keyexpr_t ke;
        z_view_keyexpr_from_str_unchecked(&ke, KEYEXPR);
        if (z_declare_subscriber(z_loan(s), &sub, z_loan(ke), z_move(callback), NULL) < 0) {
            printf("Unable to declare subscriber.\n");
            exit(-1);
        }
        printf(" OK!\n");
        printf("Waiting for messages...\n");
        while (1) {
            sleep(1);
        }
    }
    vTaskDelete(NULL);
}

void app_main() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    
    // Create the Zenoh publisher task with extra stack size (8192 bytes)
    xTaskCreate(zenoh_subscriber_task, "zenoh_sub_task", 8192, NULL, 5, NULL);
}

#else
void app_main() {
    printf("ERROR: Zenoh pico was compiled without Z_FEATURE_SUBSCRIPTION.\n");
}
#endif