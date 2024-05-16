#pragma once
#include "config/profiles.h"
#include "config/boards.h"

// configuration profile
// one of [ TINY, SMALL, FULL ]
// use SMALL unless you know what you're doing
#ifndef CONFIG_PROFILE
  #define CONFIG_PROFILE CONFIG_PROFILE_SMALL
#endif

// board selection
#ifndef BOARD
  #define BOARD BOARD_VOXELAB_V101
#endif

// include configurations
#include "config_adv.h"
#include "config/profile.h"
#include "config/board.h"
