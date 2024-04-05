#pragma once
#include <stdint.h>
#include <hc32_ddl.h>
#include "../config.h"
#include "../util.h"
#include "gpio.h"

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
  Serial(M4_USART_TypeDef *peripheral, const gpio::pin_t tx_pin) : 
    peripheral(peripheral), 
    tx_pin(tx_pin),
    rx_pin({ }),
    has_rx(false) {}
  
  /**
   * @brief Construct a new Serial object
   * @param peripheral the peripheral
   * @param tx_pin the tx pin
   * @param rx_pin the rx pin
   */
  Serial(M4_USART_TypeDef *peripheral, const gpio::pin_t tx_pin, const gpio::pin_t rx_pin) : 
    peripheral(peripheral), 
    tx_pin(tx_pin),
    rx_pin(rx_pin),
    has_rx(true) {}

  /**
   * @brief initialize the Serial
   * @param baudrate the baudrate 
   */
  void init(uint32_t baudrate);

  /**
   * @brief deinitialize the Serial
   */
  void deinit();  

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

  /**
   * @brief read data from the Serial
   * @param buffer the buffer to read into
   * @param length the maximum length to read
   * @param timeout how long to wait for the next character, in milliseconds
   * @return the number of bytes read
   */
  size_t read(uint8_t *buffer, const size_t length, const uint16_t timeout = 100);

private:
  M4_USART_TypeDef *peripheral;
  const gpio::pin_t tx_pin;
  const gpio::pin_t rx_pin;
  const bool has_rx = false;
};

#if HAS_SERIAL(HOST_SERIAL)
  extern Serial hostSerial;
#endif

#if HAS_SERIAL(SCREEN_SERIAL)
  extern Serial screenSerial;
#endif
