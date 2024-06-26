#include "log.h"
#include <stdint.h>
#include <algorithm>
#include "serial.h"
#include "screen.h"

extern "C" int printf(const char *format, ...)
{
  logging::log("[");
  logging::log(format);
  logging::log("]\n");
  return 0;
}

namespace logging
{
  namespace formatters
  {
    int format_number(char *str, uint32_t number, const int base)
    {
      // convert the number to a string
      int i = 0;
      do {
        switch(base)
        {
          case 2:
          {
            str[i++] = '0' + (number % base);
            number >>= 1; // same as number /= 2
            break;
          }
          case 10:
          {
            str[i++] = '0' + (number % base);
            number /= 10;
            break;
          }
          case 16:
          {
            const uint8_t digit = number % 16;
            str[i++] = digit < 10 ? ('0' + digit) : ('A' + (digit - 10));
            number /= 16;
            break;
          }
        } 
      } while(number != 0);

      // add padding for hex format
      if (base == 16)
      {
        while (i < 8)
        {
          str[i++] = '0';
        }
      }

      // reverse the string
      std::reverse(str, str + i);

      // add null terminator
      str[i] = '\0';
      return i;
    }
  }

  void log(const char *message, const bool to_screen)
  {
    #if HAS_SERIAL(HOST_SERIAL)
      hostSerial.write(message);
    #endif

    if (to_screen)
    {
      screen.write(message);
      screen.flush();
    }
  }

  void log(uint32_t number, const int base, const bool to_screen)
  {
    char buffer[32 + 1]; // maximum length of a 32 bit number in binary + null terminator
    formatters::format_number(buffer, number, base);
    log(buffer, to_screen);
  }
}
