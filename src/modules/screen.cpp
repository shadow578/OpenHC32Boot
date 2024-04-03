#include "screen.h"

#if IS_SCREEN(SCREEN_NONE)
  NoneScreen screen; 
#elif IS_SCREEN(SCREEN_DWIN)
  DwinScreen screen;
#else
  #error "Invalid screen selected"
#endif
