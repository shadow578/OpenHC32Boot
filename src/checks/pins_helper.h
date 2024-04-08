#pragma once
#include "../modules/gpio.h"

constexpr bool operator ==(const gpio::pin_t &first, const gpio::pin_t &other) 
{
  return first.pin == other.pin && first.port == other.port;
}
