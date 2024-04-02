#include "screen.h"
#include "../config.h"
#include "../util.h"

#if IS_SCREEN(SCREEN_NONE)
  #include "./screens/none/NoneScreen.h"
  const NoneScreen screenImpl = NoneScreen(); 
#elif IS_SCREEN(SCREEN_DWIN)
  #include "./screens/dwin/DwinScreen.h"
  const DwinScreen screenImpl = DwinScreen();
#else
  #error "Invalid screen driver selected!"
#endif

const Screen *screen = &screenImpl;
