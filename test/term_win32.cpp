/**
 * Copyright (c) 2023 Oli Legat <http://github.com/olegat>.
 * Licensed under the BSD 3-Clause License.
 */

#include "term.hpp"

#include <windows.h>

constexpr WORD default_color = 0x07; // white on black
static    WORD current_color = default_color;

static HANDLE get_console_handle()
{
  static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  return hConsole;
}

static WORD get_attributes(term::color col, WORD mask)
{
  switch (col) {
    case term::color::Current:  return current_color & mask;
    case term::color::Reset:    return default_color & mask;
    default: break;
  }
  WORD attr = static_cast<WORD>(col);
  return (attr | (attr<<4)) & mask;
}

static WORD get_attributes(const term::style& styl) {
  const WORD fgbits = get_attributes( styl.foreground, 0x0F );
  const WORD bgbits = get_attributes( styl.background, 0xF0 );
  return fgbits | bgbits;
}

bool term::istty()
{
  return get_console_handle() != INVALID_HANDLE_VALUE;
}

void term::write(const term::style& styl)
{
  SetConsoleTextAttribute( get_console_handle(),
                           get_attributes( styl ));
}
