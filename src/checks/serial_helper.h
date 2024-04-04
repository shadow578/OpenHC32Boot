#pragma once

constexpr bool is_valid_usart(const int usart, const bool allow_none = false)
{
  return (usart == 1 || usart == 2 || usart == 3 || usart == 4 || (usart == -1 && allow_none));
}
