#pragma once
#include <WVariant.h>

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
