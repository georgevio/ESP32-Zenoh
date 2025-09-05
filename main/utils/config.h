#ifndef CONFIG_H
#define CONFIG_H

#include "secret.h"  // File HAS to be created. ALL SENSITIVE CONFGIS ARE THERE!
#include "esp_log.h" // Required for ESP_LOG_INFO

// Unified Log level for all components. Can be overridden per-component in app_main.cpp
// LEVELS: ESP_LOG_NONE, ESP_LOG_ERROR, ESP_LOG_WARN, ESP_LOG_INFO, ESP_LOG_DEBUG, ESP_LOG_VERBOSE
#define DEFAULT_SYSTEM_LOG_LEVEL ESP_LOG_VERBOSE // not implemented yet

// The IP address of the computer running the Zenoh subscriber
//#define ZENOH_CONNECT_IP "192.168.1.105"
// The public Zenoh router address
//#define ZENOH_CONNECT_IP "demo.zenoh.io"
// this below is the demo.zenoh.io IP address
//#define ZENOH_CONNECT_IP "212.71.244.198"
//#define ZENOH_CONNECT_IP "demo.zenoh.io"
//#define ZENOH_CONNECT_IP "public.zenoh.io"
#define ZENOH_IP "192.168.178.181"

#define ZENOH_PROTOCOL "udp" // "udp" or "tcp". Start with udp for firewall issues!
#define ZENOH_PORT "7447" // Change for firewall reasons. An easy choice might be 8080 or SQL ports

/* Time & Timezone setting. 
 * For AWS, UTC is strongly recommended.
 * A list of timezone strings:
 * https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
 * * Tried to automate the TIMEZONE, but it is complicated and rather
 * computation consuming, so kept it manual: CET-1 Central Europe.
 */
#define TIMEZONE "CET-1CEST,M3.5.0,M10.5.0/3" // UTC+1 or CET
#define NTP_SERVER_1 "pool.ntp.org"
#define NTP_SERVER_2 "time.google.com"


#endif // CONFIG_H