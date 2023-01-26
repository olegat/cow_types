/**
 * Copyright (c) 2023 Oli Legat <http://github.com/olegat>.
 * Licensed under the BSD 3-Clause License.
 */

#include <termios.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "term.hpp"

template <class T>
using vector = std::vector<T>;
using sstream = std::stringstream;

bool term::istty() {
  static struct Init {
    bool value;
    Init() {
      struct termios attr;
      this->value = (tcgetattr(STDOUT_FILENO, &attr) == 0);
    }
  } lazyIsTTY;

  return lazyIsTTY.value;
}

static void add_colorcodes(vector<int> &codes,
                           const term::color& col,
                           int offset /* 0=foreground, 10=background */)
{
  // See this page for SGR (Select Graphic Rendition) codes:
  // https://en.wikipedia.org/wiki/ANSI_escape_code
  switch (col) {
    case term::color::Reset:
      codes.push_back( 39 + offset );
    case term::color::Current:
      return;
    default:
      break;
  }

  // term::color uses the format BGR (like on Windows), but Unix terminals use
  // the format RGB. Swap the blue & red bits.
  //
  // swapping two bit:
  // https://www.geeksforgeeks.org/how-to-swap-two-bits-in-a-given-integer/
  int colcode = static_cast<int>(col);
  int swapper = (colcode & 1) ^ ((colcode >> 2) & 1);
  colcode ^= (swapper | (swapper << 2));

  codes.push_back( 38 + offset );
  codes.push_back( 5 );
  codes.push_back( colcode );
}

static string joincodes(const vector<int> &codes)
{
  sstream ss;
  const char* delim = "";
  for (int c : codes) {
    ss << delim << c;
    delim = ";";
  }
  return ss.str();
}

static string getcodes(const term::style& styl)
{
  if (styl == term::reset) {
    return "0";
  }

  vector<int> codes;
  add_colorcodes( codes, styl.foreground,  0);
  add_colorcodes( codes, styl.background, 10);

  return joincodes(codes);
}

void term::write(const term::style& styl)
{
  if (term::istty()) {
    std::cout << "\e[" << getcodes(styl) << "m";
  }
}
