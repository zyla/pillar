#include <algorithm>
#include <array>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "sql/lexer.h"

namespace sql::lexer {

static bool is_identifier_start(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool is_identifier_char(char c) {
  return is_identifier_start(c) || (c >= '0' && c <= '9');
}

struct Lexer {
  const std::string &input;
  size_t pos = 0;

  Lexer(const std::string &input) : input(input) {}

  std::optional<LocatedToken> next();
};

struct Keyword {
  std::string str;
  TokenType token_type;
};

static const std::array<Keyword, 3> KEYWORDS = {
    Keyword{"select", Select},
    Keyword{"from", From},
    Keyword{"where", Where},
};

static bool case_insensitive_equals(const std::string &a,
                                    const std::string &b) {
  return std::equal(a.begin(), a.end(), b.begin(), b.end(),
                    [](char x, char y) { return tolower(x) == tolower(y); });
}

// Returns TokenType for an identifier-like token, which may actually be a
// keyword.
static TokenType identifier_type(const std::string &ident) {
  for (auto &kw : KEYWORDS) {
    if (case_insensitive_equals(kw.str, ident)) {
      return kw.token_type;
    }
  }

  return Identifier;
}

std::optional<LocatedToken> Lexer::next() {
  // Skip whitespace
  while (true) {
    if (pos >= input.size()) {
      return std::optional<LocatedToken>();
    }

    switch (input[pos]) {
    case ' ':
    case '\n':
      pos++;
      continue;
    case '-':
      if (pos + 1 < input.size() && input[pos + 1] == '-') {
        // Skip a comment
        while (pos < input.size() && input[pos] != '\n') {
          pos++;
        }
        break;
      } else {
        pos++;
        break;
      }
    default:
      goto skipped_ws;
    }
  }
skipped_ws:
  size_t token_start = pos;

  if (is_identifier_start(input[pos])) {
    pos++;
    while (pos < input.size() && is_identifier_char(input[pos])) {
      pos++;
    }
    std::string value = input.substr(token_start, pos - token_start);
    return std::make_optional(LocatedToken{
        .start = token_start,
        .end = pos,
        .token =
            Token{
                .token_type = identifier_type(value),
                .value = value,
            },
    });
  }

  // TODO: error handling
  return std::optional<LocatedToken>();
}

std::vector<LocatedToken> lex(const std::string &input) {
  Lexer lexer(input);
  std::vector<LocatedToken> result;

  while (true) {
    auto token = lexer.next();
    if (token) {
      result.push_back(token.value());
    } else {
      break;
    }
  }

  return result;
}

} // namespace sql::lexer
