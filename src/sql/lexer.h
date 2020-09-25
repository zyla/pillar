#pragma once

#include <variant>

namespace sql::lexer {

enum Keyword {
  KW_SELECT,
  KW_FROM,
  KW_WHERE,
};

struct Ident {
  std::string ident;
};

using Token = std::variant<Keyword, Ident>;

struct LocatedToken {
  size_t start;
  size_t end;
  Token token;
};

} // namespace sql::lexer
