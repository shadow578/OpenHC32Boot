#pragma once
#include "../config.h"

#if IS_SCREEN(SCREEN_NONE)
  #include "./screens/none/NoneScreen.h"
  extern NoneScreen screen; 
#elif IS_SCREEN(SCREEN_DWIN)
  #include "./screens/dwin/DwinScreen.h"
  extern DwinScreen screen;
#endif
