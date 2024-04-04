#pragma once
#include <hc32_ddl.h>

#define ASSERT(expr, message)     \
  if (!(expr))                    \
  {                               \
    __BKPT(0);                    \
  }
