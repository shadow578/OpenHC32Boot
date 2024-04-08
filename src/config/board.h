#pragma once
#include "boards.h"

#if IS_BOARD(VOXELAB_V101)
  #include "boards/voxelab_v101.h"
#else
  #error "Unknown board"
#endif
