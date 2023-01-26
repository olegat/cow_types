/**
 * Copyright (c) 2023 Oli Legat <http://github.com/olegat>.
 * Licensed under the BSD 3-Clause License.
 */

#include <iostream>

#define private public
#include "term.hpp"

term::ostream term::out;
term::newline term::endl;

const term::style term::reset   { term::color::Reset, term::color::Reset   };
const term::style term::redfg   { term::color::Red,   term::color::Current };
const term::style term::greenfg { term::color::Green, term::color::Current };
const term::style term::grayfg  { term::color::Gray2, term::color::Current };

term::ostream& operator<<( term::ostream& out, const char* s ) {
  std::cout << s;
  return out;
}

term::ostream& operator<<( term::ostream& out, const std::string& s ) {
  std::cout << s;
  return out;
}

term::ostream& operator<<( term::ostream& out, const term::newline& eol ) {
  std::cout << std::endl;
  return out;
}

term::ostream& operator<<( term::ostream& out, const term::style& sty ) {
  if (term::istty()) {
    term::write(sty);
  }
  return out;
}

bool operator==(const term::style& lhs, const term::style& rhs) {
  return
    lhs.foreground == rhs.foreground &&
    lhs.background == rhs.background ;
}
