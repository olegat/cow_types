/**
 * Copyright (c) 2023 Oli Legat <http://github.com/olegat>.
 * Licensed under the BSD 3-Clause License.
 */

#include <string>

using string = std::string;

// Lightweight portable terminal interace for outputting color. Colors are
// automatically suppressed if stdout is not a terminal (e.g. redirected to file
// or piped to another process). This is used similarily to std::cout. Example:
//
//   void myexample() {
//     term::out
//       << term::redfg << "Red words "
//       << term::greenfg << "Green words"
//       << term::reset << term::endl
//       << "Normal words on a new line.";
//   }
//
struct term
{
  // These are based off of MSDN ConsoleColor for portability:
  // https://learn.microsoft.com/en-us/dotnet/api/system.consolecolor?view=net-7.0
  enum class color {
    Current       = -1, // Keep the current fore/background color
    Reset         = -2, // Reset the fore/background to the default color

    DarkBlack     =  0,
    DarkBlue      =  1,
    DarkGreen     =  2,
    DarkCyan      =  3,
    DarkRed       =  4,
    DarkMagenta   =  5,
    DarkYellow    =  6,
    DarkWhite     =  7,

    BrightBlack   =  8,
    BrightBlue    =  9,
    BrightGreen   = 10,
    BrightCyan    = 11,
    BrightRed     = 12,
    BrightMagenta = 13,
    BrightYellow  = 14,
    BrightWhite   = 15,

    Black   = DarkBlack,
    Blue    = DarkBlue,
    Green   = DarkGreen,
    Cyan    = DarkCyan,
    Red     = DarkRed,
    Magenta = DarkMagenta,
    Yellow  = DarkYellow,
    White   = BrightWhite,
    Gray1   = DarkWhite,
    Gray2   = BrightBlack,
  };

  struct style {
    color foreground = color::Current;
    color background = color::Current;
  };

  struct ostream { private: ostream() {} };
  struct newline { private: newline() {} };

  static const style reset;
  static const style redfg;
  static const style greenfg;
  static const style grayfg;

  static ostream out;
  static newline endl;

  static bool istty();
  static void write(const style& sty);
};

term::ostream& operator<<( term::ostream& out, const char* s );
term::ostream& operator<<( term::ostream& out, const string& s );
term::ostream& operator<<( term::ostream& out, const term::newline& eol );
term::ostream& operator<<( term::ostream& out, const term::style& sty );

bool operator==(const term::style& lhs, const term::style& rhs);
