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
   * @param progress the progress to show [0, 100] 
   */
  virtual void showProgress(const uint8_t progress) = 0;
};

/**
 * @brief global screen instance 
 */
extern const Screen *screen;
