#pragma once
#include <stdint.h>

/**
 * @brief base class for screen implementations 
 */
class Screen
{
public:

  /**
   * @brief initialize the Screen 
   */
  virtual void init() = 0;

  /**
   * @brief clear the screen 
   */
  virtual void clear() = 0;

  /**
   * @brief write a string to the screen
   * @param str the string to write 
   */
  virtual void write(const char *str) = 0;

  /**
   * @brief flush buffered writes to the screen 
   */
  virtual void flush() = 0;

  /**
   * @brief show progress bar on the screen
   * @param progress the progress to show [0 - total]
   * @param total the total amount to progress to
   * @param message the message to show with the progress bar. if nullptr, no message is shown
   */
  virtual void showProgress(const uint32_t progress, const uint32_t total = 100, const char* message = nullptr) = 0;
};
