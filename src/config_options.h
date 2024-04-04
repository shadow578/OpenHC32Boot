#pragma once

#ifdef __cplusplus
  #include "modules/gpio.h"
#endif

//
// Serial Ports
//
#define HAS_SERIAL(port) (port != 0 && port != -1)

//
// Screen drivers
//
#define SCREEN_NONE 0
#define SCREEN_DWIN 1

#define IS_SCREEN(screen) (SCREEN_DRIVER == screen)

//
// Hash Type
//
#define HASH_NONE 0
#define HASH_CRC32 1
#define HASH_SHA256 2

//
// Log Levels
//
#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO 1
#define LOG_LEVEL_ERROR 2
#define LOG_LEVEL_OFF 3

#define IS_LOG_LEVEL(level) (LOG_LEVEL <= level)
