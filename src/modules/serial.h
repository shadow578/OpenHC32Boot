#pragma once
#include <stdint.h>
#include <hc32_ddl.h>
#include "../config.h"
#include "../util.h"
#include "../pins.h"

/**
 * @brief serial communication class
 */
class Serial
{
public:
  /**
   * @brief Construct a new Serial object
   * @param peripheral the peripheral
   * @param tx_pin the tx pin
   */
  Serial(M4_USART_TypeDef *peripheral, const pins::pin_t tx_pin) : peripheral(peripheral), tx_pin(tx_pin) {}

  /**
   * @brief initialize the Serial
   * @param baudrate the baudrate 
   */
  void init(uint32_t baudrate);  

  /**
   * @brief write a byte to the Serial
   * @param ch the byte to write 
   */
  void put(const uint8_t ch);

  /**
   * @brief write a string to the Serial
   * @param str the string to write
   */
  void write(const char *str);

private:
  M4_USART_TypeDef *peripheral;
  const pins::pin_t tx_pin;
};

#if defined(HOST_SERIAL) && HOST_SERIAL != -1
  extern Serial hostSerial;
#endif

#if defined(SCREEN_SERIAL) && SCREEN_SERIAL != -1
  extern Serial screenSerial;
#endif
