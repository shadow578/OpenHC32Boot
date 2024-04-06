#pragma once
#include <stdint.h>
#include "../config.h"

namespace logging
{
  namespace formatters
  {
    /**
     * @brief format a number
     * @param str the buffer to write the formatted number to
     * @param number the number to format
     * @param base the base of the number. one of [2, 10, 16]
     * @return the number of characters written, excluding the null terminator
     * @note the buffer must be large enough to hold the largest number in the given base plus the null terminator
     *       this is [33, 11, 9] for base [2, 10, 16] respectively
     */
    int format_number(char *str, uint32_t number, const int base);
  } // namespace formatters

  /**
   * @brief log a message
   * @param message the message to log 
   */
  void log(const char *message);

  /**
   * @brief log a number
   * @param number the number to log
   * @param base the base of the number
   */
  void log(uint32_t number, const int base);


  #define _DEF_LOG_IMPL(name)                                                       \
    inline void name(const char *message) { log(message); }                         \
    inline void name(uint32_t number, const int base) { log(number, base); }

  #define _DEF_LOG_DUMMY(name)                                                      \
    inline void name(const char *message) { }                                       \
    inline void name(uint32_t number, const int base) { }


  #if IS_LOG_LEVEL(LOG_LEVEL_DEBUG)
    _DEF_LOG_IMPL(debug)
  #else
    _DEF_LOG_DUMMY(debug)
  #endif

  #if IS_LOG_LEVEL(LOG_LEVEL_INFO)
    _DEF_LOG_IMPL(info)
  #else
    _DEF_LOG_DUMMY(info)
  #endif

  #if IS_LOG_LEVEL(LOG_LEVEL_ERROR)
    _DEF_LOG_IMPL(error)
  #else
    _DEF_LOG_DUMMY(error)
  #endif
} // namespace log
