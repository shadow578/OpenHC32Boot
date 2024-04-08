#pragma once
#include "profiles.h"

#if IS_PROFILE(FULL)
  #include "profiles/full.h"
#elif IS_PROFILE(SMALL)
  #include "profiles/small.h"
#elif IS_PROFILE(TINY)
  #include "profiles/tiny.h"
#else
  #error "Unknown configuration profile"
#endif
