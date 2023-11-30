#pragma once
#include "../config.h"
#if ENABLE_UI == 1

#include <stdint.h>

/**
 * @brief screen color type
 */
typedef uint16_t screen_color_t;

#define R_COMPONENT(R) ((((R)*32 / 256) & 0x1F) << 11) // 5 bits
#define G_COMPONENT(G) ((((G)*64 / 256) & 0x63) << 5)  // 6 bits
#define B_COMPONENT(B) (((B)*32 / 256) & 0x1F)         // 5 bits

#define RGB(R, G, B) (screen_color_t(R_COMPONENT(R) | G_COMPONENT(G) | B_COMPONENT(B)))
constexpr screen_color_t COLOR_WHITE = 0xFFFF;
constexpr screen_color_t COLOR_BLACK = 0x0000;
constexpr screen_color_t COLOR_RED = RGB(0xff, 0x00, 0x00);
constexpr screen_color_t COLOR_GREEN = RGB(0x00, 0xff, 0x00);
constexpr screen_color_t COLOR_BLUE = RGB(0x00, 0x00, 0xff);

/**
 * @brief screen font size type
 */
typedef enum screen_font_size_t
{
    /**
     * @brief 6x12
     */
    FONT_12p = 0x00,

    /**
     * @brief 8x16
     */
    FONT_16p = 0x01,

    /**
     * @brief 10x20
     */
    FONT_20p = 0x02,

    /**
     * @brief 12x24
     */
    FONT_24p = 0x03,

    /**
     * @brief 14x28
     */
    FONT_28p = 0x04,

    /**
     * @brief 16x32
     */
    FONT_32p = 0x05,

    /**
     * @brief 20x40
     */
    FONT_40p = 0x06,

    /**
     * @brief 24x48
     */
    FONT_48p = 0x07,

    /**
     * @brief 28x56
     */
    FONT_56p = 0x08,

    /**
     * @brief 32x64
     */
    FONT_64p = 0x09,

} screen_font_size_t;

/**
 * @brief screen orientation type
 */
typedef enum screen_orientation_t
{
    /**
     * @brief 0 degrees
     */
    PORTRAIT = 0x00,

    /**
     * @brief 90 degrees
     */
    LANDSCAPE = 0x01,

    /**
     * @brief 180 degrees
     */
    PORTRAIT_INVERTED = 0x02,

    /**
     * @brief 270 degrees
     */
    LANDSCAPE_INVERTED = 0x03,
} screen_orientation_t;

/**
 * @brief screen area shift mode type
 */
typedef enum screen_area_shift_dir_t
{
    AreaMove_Left = 0x00,
    AreaMove_Right = 0x01,
    AreaMove_Up = 0x02,
    AreaMove_Down = 0x03,
} screen_area_shift_dir_t;

/**
 * @brief screen module
 */
class Screen
{
public:
    /**
     * @brief initialize the screen
     */
    static void init();

    /**
     * @brief clear the screen
     * @param color the color to clear the screen with
     */
    static void clear(const screen_color_t color);

    /**
     * @brief update the screen
     */
    static void update();

    /**
     * @brief set the brightness of the screen
     * @param brightness the brightness to set the screen to
     * @note brightness is a value between 0 and 255
     */
    static void set_brightness(const uint8_t brightness);

    /**
     * @brief set the orientation of the screen
     * @param orientation the orientation to set the screen to
     */
    static void set_orientation(const screen_orientation_t orientation);

    /**
     * @brief draw a rectangle on the screen
     * @param color the color of the rectangle
     * @param x the x coordinate of the rectangle
     * @param y the y coordinate of the rectangle
     * @param width the width of the rectangle
     * @param height the height of the rectangle
     * @param fill whether or not to fill the rectangle
     */
    static void draw_rectangle(
        const screen_color_t color,
        const uint16_t x,
        const uint16_t y,
        const uint16_t width,
        const uint16_t height,
        const bool fill = false);

    /**
     * @brief draw a string on the screen
     * @param str the string to draw
     * @param x the x coordinate of the string
     * @param y the y coordinate of the string
     * @param size the font size of the string
     * @param color the color of the string
     * @param background_color the background color of the string
     * @note string length is limited to 64 characters
     */
    static void draw_string(
        const char *str,
        const uint16_t x,
        const uint16_t y,
        const screen_font_size_t size,
        const screen_color_t color,
        const screen_color_t background_color,
        const bool draw_background = true);

    /**
     * @brief draw a progress bar on the screen
     * @param x the x coordinate of the progress bar
     * @param y the y coordinate of the progress bar
     * @param width the width of the progress bar (min border_thickness*2)
     * @param height the height of the progress bar (min 1 + border_thickness*2)
     * @param progress the progress of the progress bar (0-100)
     * @param bar_color the color of the progress bar
     * @param background_color the background color of the progress bar
     * @param border_thickness the thickness of the border of the progress bar (min 1)
     * @note progress bar is filled from left to right
     */
    static void draw_progress_bar(
        const uint16_t x,
        const uint16_t y,
        const uint16_t width,
        const uint16_t height,
        const uint8_t progress,
        const screen_color_t bar_color,
        const screen_color_t background_color,
        const uint16_t border_thickness = 2);

    /**
     * @brief move an rectangular area on the screen
     * @param x the x coordinate of the area
     * @param y the y coordinate of the area
     * @param width the width of the area
     * @param height the height of the area
     * @param mode the direction to move the area
     * @param distance the distance to move the area
     * @param fill_color the color to fill the area with
     * @note the area is filled with the background color
     */
    static void move_area(
        const uint16_t x,
        const uint16_t y,
        const uint16_t width,
        const uint16_t height,
        const screen_area_shift_dir_t direction,
        const uint16_t distance,
        const screen_color_t fill_color);
};

#endif // ENABLE_UI
