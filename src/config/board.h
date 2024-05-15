#pragma once
#include "boards.h"

#if IS_BOARD(VOXELAB_V101)
  #include "boards/voxelab_v101.h"
#elif IS_BOARD(CREALITY_E2P24S4)
  #include "boards/creality_e2p24s4.h"
#else
  #error "Unknown board"
#endif
