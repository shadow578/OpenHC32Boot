#include "DwinScreen.h"
#include "../../../util.h"

#ifndef SCREEN_ORIENTATION
  #define SCREEN_ORIENTATION portrait
#endif

constexpr dwin::color::color font_color = dwin::color::white;
constexpr dwin::font_size::font_size_t font_size = dwin::font_size::pt16;

constexpr uint8_t font_height = dwin::font_size::get_character_height(font_size);
constexpr uint8_t font_width = dwin::font_size::get_character_width(font_size);

constexpr int progress_bar_height = font_height * 2;
constexpr int progress_bar_padding_top = font_height;
constexpr dwin::color::color progress_bar_color = dwin::color::green;
constexpr dwin::color::color progress_bar_background_color = dwin::color::black;
constexpr dwin::color::color progress_bar_outline_color = dwin::color::white;

constexpr dwin::screen_orientation orientation = dwin::screen_orientation::SCREEN_ORIENTATION;
constexpr dwin::rectangle screen_area = dwin::get_screen_area(orientation);
constexpr dwin::rectangle text_area = {0, 0, screen_area.width, screen_area.height - progress_bar_height - progress_bar_padding_top};
constexpr dwin::rectangle progress_bar_area = {0, screen_area.height - progress_bar_height, screen_area.width, progress_bar_height - 1}; // -1 to avoid screen wrap-around

void DwinScreen::init()
{
  dwin::init();
  dwin::set_orientation(orientation);
  clear();
  flush();
}

void DwinScreen::clear()
{
  dwin::fill_screen(dwin::color::black);
  cursor_x = 0;
  cursor_y = 0;
}

void DwinScreen::flush()
{
  dwin::redraw();
}

void DwinScreen::write(const char *str)
{
  static char buffer[64];

  uint16_t new_cursor_x = cursor_x;
  uint16_t new_cursor_y = cursor_y;
  for (size_t i = 0; *str != '\0';)
  {
    const char c = *str++;
    bool shouldFlush = false;

    // if we encounter a newline or carriage return, move the cursor accordingly
    // and flush the buffer
    if (c == '\n')
    {
      new_cursor_x = 0;
      new_cursor_y += font_height;
      shouldFlush = true;
    }
    else
    {
      // otherwise, add the character to the buffer
      buffer[i++] = c;
      new_cursor_x += font_width;
    }

    // if this is the last character in the string, or
    // if the buffer is full, or
    // if we should flush the buffer, or
    // if the cursor X coordinate would leave the screen area,
    // then draw the string and reset the buffer
    const bool lastCharacter = *str == '\0';
    const bool bufferFull = i >= sizeof(buffer) - 1;
    const bool cursorXOutOfBounds = new_cursor_x >= (text_area.x_end() - font_width);
    if (shouldFlush || lastCharacter || bufferFull || cursorXOutOfBounds)
    {
      buffer[i] = '\0';
      dwin::draw_string(buffer, cursor_x, cursor_y, font_size, dwin::color::white);

      // if the cursor X was out of bounds, move to the next line
      if (cursorXOutOfBounds)
      {
        new_cursor_x = 0;
        new_cursor_y += font_height;
      }

      // if the cursor Y is out of bounds for the text area, move the entire text area up a line
      if (new_cursor_y >= text_area.y_end())
      {
        dwin::move_screen_area(text_area, dwin::screen_area_shift_dir::up, font_height);
        new_cursor_y -= font_height;
      }

      cursor_x = new_cursor_x;
      cursor_y = new_cursor_y;
      i = 0;
    }
  }

  dwin::redraw();
}

void DwinScreen::showProgress(const uint32_t progress, const uint32_t total, const char* message)
{
  // draw progress bar background
  dwin::draw_rectangle(progress_bar_background_color, progress_bar_area, /*fill*/ true);

  // draw progress bar
  const uint16_t progress_width = (progress * progress_bar_area.width) / total;
  const dwin::rectangle progress_bar = {progress_bar_area.x, progress_bar_area.y, progress_width, progress_bar_area.height};
  dwin::draw_rectangle(progress_bar_color, progress_bar, /*fill*/ true);

  // draw progress bar outline
  dwin::draw_rectangle(progress_bar_outline_color, progress_bar_area, /*fill*/ false);

  // draw progress message in the center of the progress bar
  if (message != nullptr)
  {
    const uint16_t message_width = strlen(message) * font_width;
    const uint16_t message_x = (progress_bar_area.width - message_width) / 2;
    const uint16_t message_y = progress_bar_area.y + (progress_bar_area.height - font_height) / 2;
    dwin::draw_string(message, message_x, message_y, font_size, font_color);
  }

  dwin::redraw();
}
