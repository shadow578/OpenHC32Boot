#pragma once
#include <hc32f460_gpio.h>

namespace gpio 
{
  struct pin_t {
    const en_port_t port;
    const en_pin_t pin;

    /**
     * @brief Set the pin as output
     */
    void asOutput() const;
    
    /**
     * @brief Set the pin as input
     * @param pullup enable the internal pullup resistor
     */
    void asInput(const bool pullup = false) const;

    /**
     * @brief Set the pin output level
     * @note requires the pin to be set as output
     */
    void write(const bool level) const;

    /**
     * @brief Get the pin input level
     * @note requires the pin to be set as input
     */
    bool read() const;

    inline void low() const { write(false); }
    inline void high() const { write(true); }
  };

  // PAx
  constexpr pin_t PA0 = { PortA, Pin00 };
  constexpr pin_t PA1 = { PortA, Pin01 };
  constexpr pin_t PA2 = { PortA, Pin02 };
  constexpr pin_t PA3 = { PortA, Pin03 };
  constexpr pin_t PA4 = { PortA, Pin04 };
  constexpr pin_t PA5 = { PortA, Pin05 };
  constexpr pin_t PA6 = { PortA, Pin06 };
  constexpr pin_t PA7 = { PortA, Pin07 };
  constexpr pin_t PA8 = { PortA, Pin08 };
  constexpr pin_t PA9 = { PortA, Pin09 };
  constexpr pin_t PA10 = { PortA, Pin10 };
  constexpr pin_t PA11 = { PortA, Pin11 };
  constexpr pin_t PA12 = { PortA, Pin12 };
  constexpr pin_t PA13 = { PortA, Pin13 };
  constexpr pin_t PA14 = { PortA, Pin14 };
  constexpr pin_t PA15 = { PortA, Pin15 };

  // PBx
  constexpr pin_t PB0 = { PortB, Pin00 };
  constexpr pin_t PB1 = { PortB, Pin01 };
  constexpr pin_t PB2 = { PortB, Pin02 };
  constexpr pin_t PB3 = { PortB, Pin03 };
  constexpr pin_t PB4 = { PortB, Pin04 };
  constexpr pin_t PB5 = { PortB, Pin05 };
  constexpr pin_t PB6 = { PortB, Pin06 };
  constexpr pin_t PB7 = { PortB, Pin07 };
  constexpr pin_t PB8 = { PortB, Pin08 };
  constexpr pin_t PB9 = { PortB, Pin09 };
  constexpr pin_t PB10 = { PortB, Pin10 };
  constexpr pin_t PB11 = { PortB, Pin11 };
  constexpr pin_t PB12 = { PortB, Pin12 };
  constexpr pin_t PB13 = { PortB, Pin13 };
  constexpr pin_t PB14 = { PortB, Pin14 };
  constexpr pin_t PB15 = { PortB, Pin15 };

  // PCx
  constexpr pin_t PC0 = { PortC, Pin00 };
  constexpr pin_t PC1 = { PortC, Pin01 };
  constexpr pin_t PC2 = { PortC, Pin02 };
  constexpr pin_t PC3 = { PortC, Pin03 };
  constexpr pin_t PC4 = { PortC, Pin04 };
  constexpr pin_t PC5 = { PortC, Pin05 };
  constexpr pin_t PC6 = { PortC, Pin06 };
  constexpr pin_t PC7 = { PortC, Pin07 };
  constexpr pin_t PC8 = { PortC, Pin08 };
  constexpr pin_t PC9 = { PortC, Pin09 };
  constexpr pin_t PC10 = { PortC, Pin10 };
  constexpr pin_t PC11 = { PortC, Pin11 };
  constexpr pin_t PC12 = { PortC, Pin12 };
  constexpr pin_t PC13 = { PortC, Pin13 };
  constexpr pin_t PC14 = { PortC, Pin14 };
  constexpr pin_t PC15 = { PortC, Pin15 };

  // PDx
  constexpr pin_t PD0 = { PortD, Pin00 };
  constexpr pin_t PD1 = { PortD, Pin01 };
  constexpr pin_t PD2 = { PortD, Pin02 };
  constexpr pin_t PD3 = { PortD, Pin03 };
  constexpr pin_t PD4 = { PortD, Pin04 };
  constexpr pin_t PD5 = { PortD, Pin05 };
  constexpr pin_t PD6 = { PortD, Pin06 };
  constexpr pin_t PD7 = { PortD, Pin07 };
  constexpr pin_t PD8 = { PortD, Pin08 };
  constexpr pin_t PD9 = { PortD, Pin09 };
  constexpr pin_t PD10 = { PortD, Pin10 };
  constexpr pin_t PD11 = { PortD, Pin11 };
  constexpr pin_t PD12 = { PortD, Pin12 };
  constexpr pin_t PD13 = { PortD, Pin13 };
  constexpr pin_t PD14 = { PortD, Pin14 };
  constexpr pin_t PD15 = { PortD, Pin15 };

  // PEx
  constexpr pin_t PE0 = { PortE, Pin00 };
  constexpr pin_t PE1 = { PortE, Pin01 };
  constexpr pin_t PE2 = { PortE, Pin02 };
  constexpr pin_t PE3 = { PortE, Pin03 };
  constexpr pin_t PE4 = { PortE, Pin04 };
  constexpr pin_t PE5 = { PortE, Pin05 };
  constexpr pin_t PE6 = { PortE, Pin06 };
  constexpr pin_t PE7 = { PortE, Pin07 };
  constexpr pin_t PE8 = { PortE, Pin08 };
  constexpr pin_t PE9 = { PortE, Pin09 };
  constexpr pin_t PE10 = { PortE, Pin10 };
  constexpr pin_t PE11 = { PortE, Pin11 };
  constexpr pin_t PE12 = { PortE, Pin12 };
  constexpr pin_t PE13 = { PortE, Pin13 };
  constexpr pin_t PE14 = { PortE, Pin14 };
  constexpr pin_t PE15 = { PortE, Pin15 };

  // PHx
  constexpr pin_t PH0 = { PortH, Pin00 };
  constexpr pin_t PH1 = { PortH, Pin01 };
  constexpr pin_t PH2 = { PortH, Pin02 };
}
