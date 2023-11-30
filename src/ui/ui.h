#pragma once
#include "../config.h"
#include <Arduino.h>
#include <stdint.h>

#if ENABLE_UI == 1

#include "../screen/screen.h"
#include <string.h>

/**
 * main ui class
 */
class UI
{
public:
    /**
     * @brief initialize ui
     */
    static void init();

    /**
     * @brief clear the screen and log
     */
    static void clear();

    /**
     * @brief write a string to the on-screen log
     * @param str the string to write
     */
    static void print(const char *str);

    /**
     * @brief show progress bar, eg. for firmware flashing progress
     * @param progress the progress to show
     * @note progress is a value between 0 and 100
     * @note progress bar is drawn on the top of the screen
     */
    static void show_progress(const uint8_t progress);

    /**
     * @brief wait for the user to press the button
     * @note this does not display anything on the screen, so print something first
     */
    static void await_button_press();

private:
    /**
     * @brief is the ui initialized?
     */
    static bool is_initialized;

    /**
     * @brief the horizontal position of the cursor
     */
    static uint16_t cursor_x;

    /**
     * @brief the line buffer
     */
    static char line[];

    /**
     * @brief print the string to the cursor (x) position, then update the cursor
     */
    static void _print(const char *str, const size_t len);

    /**
     * @brief move the cursor back to the beginning of the line
     */
    static void carriage_return();

    /**
     * @brief move the cursor down one line
     * @note
     * actually, this shifts the whole screen up by one line,
     * since texts starts at the bottom of the screen
     */
    static void line_feed();
};

#else // ENABLE_UI == 0

class UI
{
public:
    /**
     * @brief initialize ui
     */
    static inline void init() {}

    /**
     * @brief clear the screen and log
     */
    static inline void clear() {}

    /**
     * @brief write a string to the on-screen log
     * @param str the string to write
     */
    static inline void print(const char *str) {}

    /**
     * @brief show progress bar, eg. for firmware flashing progress
     * @param progress the progress to show
     * @note progress is a value between 0 and 100
     * @note progress bar is drawn on the top of the screen
     */
    static inline void show_progress(const uint8_t progress) {}

    /**
     * @brief wait for the user to press the button
     * @note this does not display anything on the screen, so print something first
     */
    static inline void await_button_press()
    {
        printf("10s before continue\n");
        for (int i = 0; i < 100; i++)
        {
            delay(100);
        }
    }
};

#endif // ENABLE_UI
