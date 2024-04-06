#pragma once
#include <stdint.h>
#include <stdio.h>
#include "../../../config.h"

namespace dwin
{
  namespace constants
  {
    /**
     * @brief FHONE byte
     */
    constexpr uint8_t FHONE = 0xAA;
    
    /**
     * @brief header send before serial command data
    */
    constexpr uint8_t head[] = { FHONE };

    /**
     * @brief tail send after serial command data
     */
    constexpr uint8_t tail[] = { 0xCC, 0x33, 0xC3, 0x3C };

    /**
     * @brief delays for different operations.
     * @note set to 0 to disable
     */
    namespace delays
    {
      constexpr uint32_t byte_tx = 1; // us

      constexpr uint32_t init = 250; // ms
      constexpr uint32_t fill_screen = 100; // ms
      constexpr uint32_t redraw = 10; // ms
      constexpr uint32_t set_brightness = 0; // ms
      constexpr uint32_t set_orientation = 0; // ms
      constexpr uint32_t draw_rectangle = 5; // ms
      constexpr uint32_t draw_string = 5; // ms
      constexpr uint32_t move_screen_area = 25; // ms
    } // namespace delays
  } // namespace constants

  struct rectangle
  {
    int x;
    int y;
    int width;
    int height;

    int x_end() const { return x + width; }
    int y_end() const { return y + height; }
  };

  namespace color
  {
    /**
     * @brief DWIN color type
     */
    typedef uint16_t color;

    /**
     * @brief get a color from RGB values
     * @param r red value [0, 255]
     * @param g green value [0, 255]
     * @param b blue value [0, 255]
     */
    constexpr color get(uint8_t r, uint8_t g, uint8_t b)
    {
      r = (r * 32 / 256) & 0x1F; // 5 bits
      g = (g * 64 / 256) & 0x63; // 6 bits
      b = (b * 32 / 256) & 0x1F; // 5 bits
      return color((r << 11) | (g << 5) | b);
    }

    constexpr color white = 0xFFFF;
    constexpr color black = 0x0000;
    constexpr color red = get(0xff, 0x00, 0x00);
    constexpr color green = get(0x00, 0xff, 0x00);
    constexpr color blue = get(0x00, 0x00, 0xff);
  } // namespace color

  namespace font_size 
  {
    enum font_size_t : uint8_t
    {
      /**
       * @brief 6x12
       */
      pt12 = 0x00,

      /**
       * @brief 8x16
       */
      pt16 = 0x01,

      /**
       * @brief 10x20
       */
      pt20 = 0x02,

      /**
       * @brief 12x24
       */
      pt24 = 0x03,

      /**
       * @brief 14x28
       */
      pt28 = 0x04,

      /**
       * @brief 16x32
       */
      pt32 = 0x05,

      /**
       * @brief 20x40
       */
      pt40 = 0x06,

      /**
       * @brief 24x48
       */
      pt48 = 0x07,

      /**
       * @brief 28x56
       */
      pt56 = 0x08,

      /**
       * @brief 32x64
       */
      pt64 = 0x09,
    };

    /**
     * @brief get the width of a character in a font
     */
    constexpr uint8_t get_character_width(const font_size_t font)
    {
      switch (font)
      {
        case pt12: return 6;
        case pt16: return 8;
        case pt20: return 10;
        case pt24: return 12;
        case pt28: return 14;
        case pt32: return 16;
        case pt40: return 20;
        case pt48: return 24;
        case pt56: return 28;
        case pt64: return 32;
        default: return 0;
      }
    }

    /**
     * @brief get the height of a character in a font
     */
    constexpr uint8_t get_character_height(const font_size_t font)
    {
      switch (font)
      {
        case pt12: return 12;
        case pt16: return 16;
        case pt20: return 20;
        case pt24: return 24;
        case pt28: return 28;
        case pt32: return 32;
        case pt40: return 40;
        case pt48: return 48;
        case pt56: return 56;
        case pt64: return 64;
        default: return 0;
      }
    }
  } // namespace font_size

  enum class screen_orientation : uint8_t
  {
    /**
     * @brief 0 degrees
     */
    landscape = 0x00,

    /**
     * @brief 90 degrees
     */
    portrait = 0x01,

    /**
     * @brief 180 degrees
     */
    landscape_inverted = 0x02,

    /**
     * @brief 270 degrees
     */
    portrait_inverted = 0x03,
  };

  enum class screen_area_shift_dir : uint8_t
  {
    left = 0x00,
    right = 0x01,
    up = 0x02,
    down = 0x03,
  };

  /**
   * @brief get the screen area for a given orientation
   * @param orientation the orientation to get the screen area for
   * @return the screen area
   */
  constexpr rectangle get_screen_area(const screen_orientation orientation)
  {
    constexpr struct { int width; int height; } dimensions = SCREEN_DIMENSIONS;

    switch (orientation)
    {
      case screen_orientation::portrait:
      case screen_orientation::portrait_inverted:
        return {0, 0, dimensions.width, dimensions.height};
      case screen_orientation::landscape:
      case screen_orientation::landscape_inverted:
        return {0, 0, dimensions.height, dimensions.width};
      default:
        return {0, 0, 0, 0};
    }
  }

  /**
   * @brief initialize the DWIN screen
   */
  void init();

  /**
   * @brief clear the screen
   * @param color the color to clear the screen with
   */
  void fill_screen(const color::color color);

  /**
   * @brief force a screen redraw
   */
  void redraw();

  /**
   * @brief set the brightness of the screen
   * @param brightness the brightness to set the screen to
   * @note brightness is a value between 0 and 255
   */
  void set_brightness(const uint8_t brightness);

  /**
   * @brief set the orientation of the screen
   * @param orientation the orientation to set the screen to
   */
  void set_orientation(const screen_orientation orientation);
  
  /**
   * @brief draw a rectangle on the screen
   * @param color the color of the rectangle
   * @param rectangle the rectangle to draw
   * @param fill whether or not to fill the rectangle
   */
  void draw_rectangle(
    const color::color color,
    const rectangle &rectangle,
    const bool fill = false);

  /**
   * @brief draw a string on the screen
   * @param str the string to draw
   * @param x the x coordinate of the string
   * @param y the y coordinate of the string
   * @param font the font size of the string
   * @param color the color of the string
   * @param background the background color of the string
   * @param draw_background whether or not to draw the background
   * @return the number of characters drawn
   */
  size_t draw_string(
    const char *str,
    const uint16_t x,
    const uint16_t y,
    const font_size::font_size_t font = font_size::pt16,
    const color::color color = color::white,
    const color::color background = color::black,
    const bool draw_background = false);
  
  /**
   * @brief move a screen area in a direction
   * @param area the area to move
   * @param dir the direction to move the area
   * @param amount the amount to move the area
   * @param fill_color the color to fill the area with
   */
  void move_screen_area(
    const rectangle &area,
    const screen_area_shift_dir dir,
    const uint16_t amount,
    const color::color fill_color = color::black);
} // namespace dwin
