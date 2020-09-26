#include "gtest/gtest.h"

#include "sql/lexer.h"

TEST(lexer, identifiers) {
  auto tokens = sql::lexer::lex("foo bar_baz qux123");
  ASSERT_EQ(pprint::pprint_multiline(tokens),
            R"([
  LocatedToken {
    start: 0,
    end: 3,
    token: Token {
      token_type: Identifier,
      value: "foo",
    },
  },
  LocatedToken {
    start: 4,
    end: 11,
    token: Token {
      token_type: Identifier,
      value: "bar_baz",
    },
  },
  LocatedToken {
    start: 12,
    end: 18,
    token: Token {
      token_type: Identifier,
      value: "qux123",
    },
  },
])");
}
