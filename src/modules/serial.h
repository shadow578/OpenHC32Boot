#pragma once
#include <stdint.h>

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
   */
  void init();  

  /**
   * @brief write a byte to the Serial
   * @param ch the byte to write 
   */
  void write(const uint8_t ch);
};

#if defined(HOST_SERIAL) && HOST_SERIAL != -1
  const Serial hostSerial(CONCAT(M4_Usart, HOST_SERIAL), HOST_SERIAL_TX);
#endif

#if defined(SCREEN_SERIAL) && SCREEN_SERIAL != -1
  const Serial screenSerial();
#endif
