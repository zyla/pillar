#pragma once

#include <variant>

#include "util/pprint.h"

namespace sql::lexer {

struct Keyword {
  enum {
    SELECT,
    FROM,
    WHERE,
  } value;

  void pprint_write(pprint::Writer &w) {
    switch (this->value) {
    case Keyword::SELECT:
      w.write("Keyword::SELECT");
      break;
    case Keyword::FROM:
      w.write("Keyword::FROM");
      break;
    case Keyword::WHERE:
      w.write("Keyword::WHERE");
      break;
    }
  }
};

struct Ident {
  std::string ident;

  void pprint_write(pprint::Writer &w) {
    w.open_struct("Ident");
    w.field("ident", this->ident);
    w.close_struct();
  }
};

using Token = std::variant<Keyword, Ident>;

struct LocatedToken {
  size_t start;
  size_t end;
  Token token;
};

} // namespace sql::lexer
