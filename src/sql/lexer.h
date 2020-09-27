#pragma once

#include <variant>

#include "util/pprint.h"

namespace sql::lexer {

struct unit {
  void pprint_write(pprint::Writer &w) const { w.write("unit"); }
};

enum class TokenType {
  Select,
  From,
  Where,

  Identifier,
};

inline const char *token_type_to_string(TokenType t) {
  switch (t) {
  case TokenType::Select:
    return "Select";
  case TokenType::From:
    return "From";
  case TokenType::Where:
    return "Where";
  case TokenType::Identifier:
    return "Identifier";
  }
}

struct Token {
  TokenType token_type;
  std::variant<unit, int, std::string> value;

  void pprint_write(pprint::Writer &w) const {
    w.open_struct("Token");
    w.field("token_type",
            pprint::raw_string(token_type_to_string(this->token_type)));
    w.field("value", this->value);
    w.close_struct();
  }
};

struct LocatedToken {
  size_t start;
  size_t end;
  Token token;

  void pprint_write(pprint::Writer &w) const {
    w.open_struct("LocatedToken");
    w.field("start", this->start);
    w.field("end", this->end);
    w.field("token", this->token);
    w.close_struct();
  }
};

std::vector<LocatedToken> lex(const std::string &input);

} // namespace sql::lexer
