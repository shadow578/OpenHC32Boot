#pragma once
#include "../../screen.h"
#include "../../serial.h"

/**
 * @brief DWIN screen implementation
 */
class DwinScreen : public Screen
{
public:
  DwinScreen(const Serial *serial);
  
  void init() override;
  void clear() override;
  void write(const char *str) override;
  void flush() override;
  void showProgress(const uint8_t progress) override;
};