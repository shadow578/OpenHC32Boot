#include "ui.h"
#if ENABLE_UI == 1

#include <Arduino.h>

//
// shared
//
constexpr screen_color_t CLEAR_COLOR = COLOR_BLACK; // color the screen is cleared to
constexpr uint16_t PADDING = 4;                     // general padding between screen edges and elements

//
// progress style
//
constexpr uint16_t PROGRESS_BAR_HEIGHT = 24;             // height of the progress bar
constexpr screen_color_t PROGRESS_BAR_COLOR = 0xFE29;    // color of the progress bar
constexpr screen_color_t PROGRESS_BAR_BG_COLOR = 0x10E4; // color of the progress bar background

//
// print style
//
constexpr screen_font_size_t PRINT_FONT_SIZE = FONT_16p; // font size for print()
constexpr uint16_t PRINT_FONT_H = 16;                    // height of the font used for print()
constexpr uint16_t PRINT_FONT_W = 8;                     // width of the font used for print()
constexpr screen_color_t PRINT_TEXT_COLOR = COLOR_WHITE; // color of the text printed with print()

//
// computed values
//
constexpr uint16_t PRINT_AREA_X = PADDING;                                             // start at the left edge of the screen + padding
constexpr uint16_t PRINT_AREA_Y = PROGRESS_BAR_HEIGHT + PADDING;                       // start below the progress bar + padding
constexpr uint16_t PRINT_AREA_W = SCREEN_WIDTH - (PADDING * 2);                        // fill the whole screen width (minus padding)
constexpr uint16_t PRINT_AREA_H = SCREEN_HEIGHT - PROGRESS_BAR_HEIGHT - (PADDING * 2); // fill the whole screen height, minus the progress bar and paddings

constexpr uint16_t PRINT_LINE_START_X = PRINT_AREA_X;                           // start at the left edge of the screen + padding
constexpr uint16_t PRINT_LINE_START_Y = SCREEN_HEIGHT - PRINT_FONT_H - PADDING; // start at the bottom of the screen - padding

constexpr uint16_t MAX_CHARACTERS_PER_LINE = PRINT_AREA_W / PRINT_FONT_W; // how many characters can fit on a line?

//
// UI implementation
//
bool UI::is_initialized = false;
uint16_t UI::cursor_x = 0;
char UI::line[MAX_CHARACTERS_PER_LINE + 1];

void UI::init()
{
    Screen::init();
    is_initialized = true;
    clear();

    // init button
    pinMode(PB14, UI_BUTTON_MODE);
}

void UI::clear()
{
    if (!is_initialized)
    {
        return;
    }

    Screen::clear(CLEAR_COLOR);
    Screen::update();

    // return cursor
    carriage_return();
    line_feed();
}

void UI::print(const char *str)
{
    if (!is_initialized)
    {
        return;
    }

    // copy the string to a buffer, as we need to handle line feeds & carriage returns separately
    // also, we have to split strings that are too long into multiple lines

    // copy each character to the buffer, keeping track of length
    size_t len = 0;
    for (size_t i = 0;; i++)
    {
        // get character
        char c = str[i];

        // end if null terminator
        if (c == '\0')
        {
            break;
        }

        // if the character is carriage return, reset to the beginning of the line
        if (c == '\r')
        {
            len = 0;
            carriage_return();
            continue;
        }

        // if the character is a newline, or if the line is longer than the screen can display,
        // print it and feed to the next line
        if (c == '\n' || (cursor_x + len) >= MAX_CHARACTERS_PER_LINE)
        {
            // add null terminator
            line[len] = '\0';

            // print the line to the screen
            _print(line, len);

            // reset line length
            len = 0;

            // feed screen to the next line
            carriage_return();
            line_feed();
            continue;
        }

        // add character to the buffer
        line[len++] = c;
    }

    // if there are more than 0 characters left in the buffer, print them
    if (len > 0)
    {
        // add null terminator
        line[len] = '\0';

        // print the line to the screen
        _print(line, len);
    }

    // finally, update the screen
    Screen::update();
}

void UI::show_progress(const uint8_t progress)
{
    if (!is_initialized)
    {
        return;
    }

    Screen::draw_progress_bar(
        PADDING,
        PADDING,
        SCREEN_WIDTH - (PADDING * 2),
        PROGRESS_BAR_HEIGHT,
        progress,
        PROGRESS_BAR_COLOR,
        PROGRESS_BAR_BG_COLOR,
        2);
    Screen::update();
}

void UI::await_button_press()
{
#if UI_BUTTON_MODE == INPUT // active HIGH
#define UI_BUTTON_RELASED_STATE LOW
#define UI_BUTTON_PRESSED_STATE HIGH
#elif UI_BUTTON_MODE == INPUT_PULLUP // active LOW
#define UI_BUTTON_RELASED_STATE HIGH
#define UI_BUTTON_PRESSED_STATE LOW
#else
#error "UI_BUTTON_MODE not defined or invalid"
#endif

    // wait for press
    while (digitalRead(PB14) == UI_BUTTON_RELASED_STATE)
    {
        delay(10);
    }

    // wait for release
    while (digitalRead(PB14) == UI_BUTTON_PRESSED_STATE)
    {
        delay(10);
    }
}

//
// private
//

#include <stdio.h>

void UI::_print(const char *str, const size_t len)
{
    // draw the string to the screen
    Screen::draw_string(
        str,
        PRINT_LINE_START_X + (cursor_x * PRINT_FONT_W),
        PRINT_LINE_START_Y,
        PRINT_FONT_SIZE,
        PRINT_TEXT_COLOR,
        CLEAR_COLOR,
        true);

    // update cursor
    cursor_x += len;
}

void UI::carriage_return()
{
    cursor_x = 0;
}

void UI::line_feed()
{
    Screen::move_area(
        PRINT_AREA_X,
        PRINT_AREA_Y,
        PRINT_AREA_W,
        PRINT_AREA_H,
        AreaMove_Up,
        PRINT_FONT_H,
        CLEAR_COLOR);
}

#endif // ENABLE_UI == 1
