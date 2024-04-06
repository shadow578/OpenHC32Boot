#pragma once
#include "../../serial.h"
#include "../Screen.h"
#include "dwin.h"

/**
 * @brief DWIN screen implementation
 */
class DwinScreen : public Screen
{
public: 
  void init() override;
  void clear() override;
  void write(const char *str) override;
  void flush() override;
  void showProgress(const uint32_t progress, const uint32_t total = 100, const char* message = nullptr) override;

private:
  uint16_t cursor_x, cursor_y;

  dwin::rectangle text_area;
  dwin::rectangle progress_bar_area;
};
