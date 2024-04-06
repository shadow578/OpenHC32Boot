#pragma once
#include "../Screen.h"

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
  void showProgress(const uint32_t progress, const uint32_t total = 100, const char* message = nullptr) override {}
};