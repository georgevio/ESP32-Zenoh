/* FILE ALTERED AND ADAPTED FROM THE ORIGINAL BELOW 
 * georgevio@gmail.com
 */

// Copyright (c) 2022 ZettaScale Technology
//
// This program and the accompanying materials are made available under the
// terms of the Eclipse Public License 2.0 which is available at
// http://www.eclipse.org/legal/epl-2.0, or the Apache License, Version 2.0
// which is available at https://www.apache.org/licenses/LICENSE-2.0.
//
// SPDX-License-Identifier: EPL-2.0 OR Apache-2.0
//
// Contributors:
//   ZettaScale Zenoh Team, <zenoh@zettascale.tech>

#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
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

#if Z_FEATURE_PUBLICATION == 1

#define MODE "client"
#define KEYEXPR "esp32/public/test"
#define VALUE "Hello from ESP32"

// Zenoh publisher task
void zenoh_publisher_task(void* pvParameters) {
    // function moved to wifi.c
    wifi_init_sta();

    // URL in config.h
    char locator[128];
    sprintf(locator, "udp/%s:7447", ZENOH_CONNECT_IP);

    while (1) {
        printf("\n--- Zenoh-Pico Publisher ---\n");
        printf("Mode: %s\n", MODE);
        printf("Connecting to: %s\n", locator);
        printf("Publishing on Key: %s\n", KEYEXPR);

        z_owned_config_t config;
        z_config_default(&config);
        zp_config_insert(z_loan_mut(config), Z_CONFIG_MODE_KEY, MODE);
        zp_config_insert(z_loan_mut(config), Z_CONFIG_CONNECT_KEY, locator);

        printf("Attempting to open Zenoh session...\n");
        z_owned_session_t s;
        if (z_open(&s, z_move(config), NULL) < 0) {
            printf("Error: Zenoh failed. Retry in 29 sec...\n");
            vTaskDelay(pdMS_TO_TICKS(29000));
            continue;
        }

        printf("Zenoh session opened successfully!\n");

        zp_start_read_task(z_loan_mut(s), NULL);
        zp_start_lease_task(z_loan_mut(s), NULL);

        printf("Declaring publisher for '%s'...", KEYEXPR);
        z_owned_publisher_t pub;
        z_view_keyexpr_t ke;
        z_view_keyexpr_from_str_unchecked(&ke, KEYEXPR);
        if (z_declare_publisher(z_loan(s), &pub, z_loan(ke), NULL) < 0) {
            printf("Unable to declare publisher!\n");
            exit(-1);
        }
        printf(" OK\n\n");

        char buf[256];
        for (int idx = 0; 1; ++idx) {
            sleep(5);
            sprintf(buf, "%s No #%d", VALUE, idx);
            printf(">>> Sending: '%s'\n", buf);

            z_owned_bytes_t payload;
            z_bytes_copy_from_str(&payload, buf);
            z_publisher_put(z_loan(pub), z_move(payload), NULL);
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
    xTaskCreate(zenoh_publisher_task, "zenoh_pub_task", 8192, NULL, 5, NULL);
}

#else
void app_main() {
    printf("ERROR: Zenoh pico was compiled without Z_FEATURE_PUBLICATION.\n");
}
#endif