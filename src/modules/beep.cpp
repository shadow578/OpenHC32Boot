#include "beep.h"
#include "delay.h"
#include "../config.h"

namespace beep
{
  void beep(const uint32_t duration, const uint32_t repeat)
  {
    #if defined(BEEPER_PIN)
      constexpr gpio::pin_t beeper = BEEPER_PIN;

      beeper.asOutput();
      
      for (uint32_t i = 0; i < repeat; i++)
      {
        beeper.high();
        delay::ms(duration);
        beeper.low();
        delay::ms(duration);
      }
    #endif
  }
} // namespace beep
