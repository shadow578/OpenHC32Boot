#include "screen.h"

#if ENABLE_UI == 1
#include "../serial/serial.h"

//
// utility functions
//
constexpr uint8_t DWIN_HEAD[] = {0xAA};
constexpr uint8_t DWIN_TAIL[] = {0xCC, 0x33, 0xC3, 0x3C};

void dwin_send(const uint8_t *data, const uint16_t len)
{
    // send head
    for (uint8_t i = 0; i < sizeof(DWIN_HEAD); i++)
    {
        SCREEN_PUTCHAR(DWIN_HEAD[i]);
    }

    // send data
    for (uint16_t i = 0; i < len; i++)
    {
        SCREEN_PUTCHAR(data[i]);
    }

    // send tail
    for (uint8_t i = 0; i < sizeof(DWIN_TAIL); i++)
    {
        SCREEN_PUTCHAR(DWIN_TAIL[i]);
    }
}

/**
 * @brief 16 bit word to 2 bytes
 */
#define DWIN_WORD(x) uint8_t(((x) >> 8) & 0xFF), uint8_t((x)&0xFF)

//
// screen implementation
//
void Screen::init()
{
    // serial is already initialized at this point

    // send handshake, don't care about response
    dwin_send(0x00, 1);

    // set brightness to 100%
    set_brightness(255);

    // set orientation to value in config.h
    set_orientation(SCREEN_ORIENTATION);

    // clear screen in white
    // clear(COLOR_WHITE);

    // update screen
    update();
}

void Screen::clear(const screen_color_t color)
{
    uint8_t data[] = {
        0x01, // cmd: clear screen
        DWIN_WORD(color),
    };

    dwin_send(data, sizeof(data));
}

void Screen::update()
{
    uint8_t data[] = {
        0x3D, // cmd: update screen
    };

    dwin_send(data, sizeof(data));

    // wait for screen to update
    delay(10);
}

void Screen::set_brightness(const uint8_t brightness)
{
    uint8_t data[] = {
        0x30, // cmd: set brightness
        brightness,
    };

    dwin_send(data, sizeof(data));
}

void Screen::set_orientation(const screen_orientation_t orientation)
{
    uint8_t data[] = {
        0x34, // cmd: set orientation
        0x5A,
        0xA5,
        orientation,
    };

    dwin_send(data, sizeof(data));
}

void Screen::draw_rectangle(
    const screen_color_t color,
    const uint16_t x,
    const uint16_t y,
    const uint16_t width,
    const uint16_t height,
    const bool fill)
{
    uint8_t data[] = {
        0x05,                  // cmd: draw rectangle
        uint8_t(fill ? 1 : 0), // fill mode
        DWIN_WORD(color),      // rectangle color
        DWIN_WORD(x),          // x start (top left corner)
        DWIN_WORD(y),          // y start
        DWIN_WORD(x + width),  // x end (bottom right corner)
        DWIN_WORD(y + height), // y end
    };

    dwin_send(data, sizeof(data));
}

void Screen::draw_string(
    const char *str,
    const uint16_t x,
    const uint16_t y,
    const screen_font_size_t size,
    const screen_color_t color,
    const screen_color_t background_color,
    const bool draw_background)
{
    // calculate buffer size
    const size_t str_len = min(strlen(str), 64);
    const size_t data_len = 1   // cmd
                            + 1 // flags/size
                            + 2 // fg color
                            + 2 // bg color
                            + 2 // x
                            + 2 // y
                            + str_len;

    // allocate buffer for dwin data
    uint8_t *data = new uint8_t[data_len];

    // set command, flags, size, fg color, bg color, x, y
    data[0] = 0x11; // cmd: draw string

    // flags:
    // Bit 7: widthAdjust (fixed to 0)
    // Bit 6: bShow (1 if background should be drawn, 0 otherwise)
    // Bit 5-4: reserved
    // Bit 3-0: size
    data[1] = ((draw_background ? 1 : 0) << 6) | (size & 0x0F);

    // fg color
    data[2] = color >> 8;
    data[3] = color & 0xFF;

    // bg color
    data[4] = background_color >> 8;
    data[5] = background_color & 0xFF;

    // x
    data[6] = x >> 8;
    data[7] = x & 0xFF;

    // y
    data[8] = y >> 8;
    data[9] = y & 0xFF;

    // copy string
    memcpy(&data[10], str, str_len);

    // send command and data
    dwin_send(data, data_len);

    // free buffer
    delete[] data;
}

void Screen::draw_progress_bar(
    const uint16_t x,
    const uint16_t y,
    const uint16_t width,
    const uint16_t height,
    const uint8_t progress,
    const screen_color_t bar_color,
    const screen_color_t background_color,
    const uint16_t border_thickness)
{
    // outer (border) rectangle is the full width + height of the progress bar
    // fill to clear out any previous progress bar
    draw_rectangle(
        background_color,
        x,
        y,
        width,
        height,
        /*fill*/ true);

    // inner (bar) rectangle subtracts the border thickness from the outer rectangle
    // width is scaled by the progress %
    const uint16_t full_width = width - (border_thickness * 2);
    draw_rectangle(
        bar_color,
        x + border_thickness,
        y + border_thickness,
        full_width * progress / 100,
        height - (border_thickness * 2),
        /*fill*/ true);
}

void Screen::move_area(
    const uint16_t x,
    const uint16_t y,
    const uint16_t width,
    const uint16_t height,
    const screen_area_shift_dir_t direction,
    const uint16_t distance,
    const screen_color_t fill_color)
{
    uint8_t data[] = {
        0x09,                          // cmd: move area
        uint8_t((1 << 7) | direction), // mode=translation; direction
        DWIN_WORD(distance),           // distance
        DWIN_WORD(fill_color),         // fill color
        DWIN_WORD(x),                  // x start (top left corner)
        DWIN_WORD(y),                  // y start
        DWIN_WORD(x + width),          // x end (bottom right corner)
        DWIN_WORD(y + height),         // y end
    };

    dwin_send(data, sizeof(data));
}

#endif // ENABLE_UI
