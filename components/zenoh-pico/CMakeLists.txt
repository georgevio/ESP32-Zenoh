# Find all the common source files
file(GLOB_RECURSE PICO_SRCS_COMMON
    "src/api/*.c"
    "src/collections/*.c"
    "src/link/*.c"
    "src/net/*.c"
    "src/protocol/*.c"
    "src/session/*.c"
    "src/transport/*.c"
    "src/utils/*.c"
    "src/system/common/*.c"
)

# Find the platform-specific files for FreeRTOS + LWIP (used by ESP-IDF)
file(GLOB_RECURSE PICO_SRCS_PLATFORM
    "src/system/freertos/lwip/*.c"
    "src/system/freertos/system.c"
)

# Combine the lists of common and platform-specific files
list(APPEND PICO_SRCS_COMMON ${PICO_SRCS_PLATFORM})

# Register the component with the specific source files.
idf_component_register(SRCS ${PICO_SRCS_COMMON}
                    INCLUDE_DIRS "include"
                    REQUIRES freertos lwip)

# Add all compile definitions to the correct __idf_zenoh-pico component target
target_compile_definitions(__idf_zenoh-pico PUBLIC
    ZENOH_FREERTOS_LWIP
    Z_LOG_LEVEL_DEFAULT=Z_LOG_LEVEL_TRACE
    # Definitions for memory and timeout fix
	Z_FRAG_MAX_SIZE=1024   
    Z_BATCH_UNICAST_SIZE=1024
    Z_BATCH_MULTICAST_SIZE=1024
 	Z_CONFIG_SOCKET_TIMEOUT=5000
)