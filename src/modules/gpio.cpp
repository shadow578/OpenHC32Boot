#include "gpio.h"

namespace gpio
{
  void pin_t::asOutput() const
  {
    stc_port_init_t init = {
      .enPinMode = Pin_Mode_Out,
    };

    PORT_Init(port, pin, &init);
  }

  void pin_t::asInput(const bool pullup) const
  {
    stc_port_init_t init = {
      .enPinMode = Pin_Mode_In,
      .enPullUp = pullup ? Enable : Disable,
    };

    PORT_Init(port, pin, &init);
  }

  void pin_t::write(const bool level) const
  {
    if (level)
    {
      PORT_SetBits(port, pin);
    }
    else
    {
      PORT_ResetBits(port, pin);
    }
  }

  bool pin_t::read() const
  {
    return PORT_GetBit(port, pin);
  }

  void pin_t::toggle() const
  {
    PORT_Toggle(port, pin);
  }
} // namespace gpio
