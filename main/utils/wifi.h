#ifndef WIFI_H
#define WIFI_H

#include "esp_err.h"

#ifdef __cplusplus 
extern "C" {       // C linkage for C++ compilers
#endif

/**
 * @brief Initialize WiFi in Station mode.
 */
void wifi_init_sta(void);

#ifdef __cplusplus
} // End of extern "C"
#endif

#endif // WIFI_H