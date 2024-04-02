#pragma once
#include "../config.h"

#if IS_SCREEN(SCREEN_NONE)
  #include "./screens/none/NoneScreen.h"
  NoneScreen screen(); 
#elif IS_SCREEN(SCREEN_DWIN)
  #include "./screens/dwin/DwinScreen.h"
  DwinScreen screen;
#else
  #error "Invalid screen driver selected!"
#endif
