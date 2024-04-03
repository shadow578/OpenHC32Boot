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
  void log(const char *message)
  {
    hostSerial.write(message);
    screen.write(message);
    screen.flush();
  }

  void log(uint32_t number, const int base)
  {
    char buffer[32 + 1]; // maximum length of a 32 bit number in binary + null terminator
    
    // convert the number to a string
    int i = 0;
    do {
      switch(base)
      {
        case 2:
        {
          buffer[i++] = '0' + (number % base);
          number >>= 1; // same as number /= 2
          break;
        }
        case 10:
        {
          buffer[i++] = '0' + (number % base);
          number /= 10;
          break;
        }
        case 16:
        {
          const uint8_t digit = number % 16;
          buffer[i++] = digit < 10 ? ('0' + digit) : ('A' + (digit - 10));
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
        buffer[i++] = '0';
      }
    }

    // reverse the string
    std::reverse(buffer, buffer + i);

    // write the string
    buffer[i] = '\0';
    log(buffer);
  }
}
