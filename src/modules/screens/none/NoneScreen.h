#pragma once
#include "../../screen.h"

/**
 * @brief NONE screen implementation
 */
class NoneScreen : public Screen
{
public:
  void init() override {}
  void clear() override {}
  void write(const char *str) override {}
  void flush() override {}
  void showProgress(const uint8_t progress) override {}
};