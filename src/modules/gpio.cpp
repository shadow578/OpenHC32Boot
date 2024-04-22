#include "gpio.h"

namespace gpio
{
  /**
   * @brief Set the mode of a pin
   * @param pin pin to set the mode of
   * @param input true if input mode, false if output mode
   * @param pullup true if pull-up enabled, false if pull-up disabled
   * @note slimmed down version of PORT_Init() function
   */
  void set_mode(const pin_t *pin, const bool input, const bool pullup)
  {
    PORT_Unlock();

    // find pin position from mask
    uint8_t pin_pos = 0u;
    while ((pin->pin & (1ul << pin_pos)) == 0)
    {
      pin_pos++;
    }

    // get GPIO registers
    volatile stc_port_pcr_field_t *PCRx = reinterpret_cast<volatile stc_port_pcr_field_t *>(
      reinterpret_cast<uint32_t>(&M4_PORT->PCRA0) + (0x40ul * pin->port) + (0x04ul * pin_pos));
    
    // pull-up setting
    PCRx->PUU = pullup ? 1u : 0u;

    // pin mode setting
    PCRx->DDIS = 0u;
    PCRx->POUTE = input ? 0u : 1u;

    PORT_Lock();
  }

  void pin_t::asOutput() const
  {
    set_mode(this, false, false);
  }

  void pin_t::asInput(const bool pullup) const
  {
    set_mode(this, true, pullup);
  }

  void pin_t::write(const bool level) const
  {
    volatile uint16_t *PODRx = reinterpret_cast<volatile uint16_t *>(
      reinterpret_cast<uint32_t>(&M4_PORT->PODRA) + (0x10u * port));

    if (level) 
    {
      *PODRx |= pin;
    }
    else
    {
      *PODRx &= ~pin;
    }
  }

  bool pin_t::read() const
  {
    return PORT_GetBit(port, pin);
  }
} // namespace gpio
