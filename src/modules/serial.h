#pragma once
#include <stdint.h>
#include <Arduino.h>
#include "../util.h"

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
  Serial(M4_USART_TypeDef *peripheral, const uint8_t tx_pin);

  /**
   * @brief initialize the Serial
   * @param baudrate the baudrate 
   */
  void init(uint32_t baudrate);  

  /**
   * @brief write a byte to the Serial
   * @param ch the byte to write 
   */
  void write(const uint8_t ch);
};

#if defined(HOST_SERIAL) && HOST_SERIAL != -1
  Serial hostSerial(CONCAT(M4_USART, HOST_SERIAL), HOST_SERIAL_TX);
#endif

#if defined(SCREEN_SERIAL) && SCREEN_SERIAL != -1
  Serial screenSerial(CONCAT(M4_USART, SCREEN_SERIAL), SCREEN_SERIAL_TX);
#endif
