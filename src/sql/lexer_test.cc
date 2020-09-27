#include "util/test.h"

#include "sql/lexer.h"

using sql::lexer::LocatedToken;
using sql::lexer::Token;

std::vector<Token> unlocate(const std::vector<LocatedToken> &tokens) {
  std::vector<Token> result;
  for (auto &lt : tokens) {
    result.push_back(lt.token);
  }
  return result;
}

TEST(lexer, identifiers) {
  ASSERT_SNAPSHOT_EQ(unlocate(sql::lexer::lex("foo bar_baz qux123")),
                     R"([
  Token {
    token_type: Identifier,
    value: "foo",
  },
  Token {
    token_type: Identifier,
    value: "bar_baz",
  },
  Token {
    token_type: Identifier,
    value: "qux123",
  },
])");
}

TEST(lexer, keywords) {
  ASSERT_SNAPSHOT_EQ(unlocate(sql::lexer::lex(
                         "select SELECT SeLeCT where from selects select1")),
                     R"([
  Token {
    token_type: Select,
    value: "select",
  },
  Token {
    token_type: Select,
    value: "SELECT",
  },
  Token {
    token_type: Select,
    value: "SeLeCT",
  },
  Token {
    token_type: Where,
    value: "where",
  },
  Token {
    token_type: From,
    value: "from",
  },
  Token {
    token_type: Identifier,
    value: "selects",
  },
  Token {
    token_type: Identifier,
    value: "select1",
  },
])");
}

TEST(lexer, ws_after_identifier) {
  ASSERT_SNAPSHOT_EQ(unlocate(sql::lexer::lex("foo  ")),
                     R"([
  Token {
    token_type: Identifier,
    value: "foo",
  },
])");
}

TEST(lexer, comments) {
  ASSERT_SNAPSHOT_EQ(
      unlocate(sql::lexer::lex(" -- This is a SQL comment bla bla bla\nfoo  ")),
      R"([
  Token {
    token_type: Identifier,
    value: "foo",
  },
])");
}

TEST(lexer, operators) {
  ASSERT_SNAPSHOT_EQ(unlocate(sql::lexer::lex(",")),
                     R"([
  Token {
    token_type: Comma,
    value: unit,
  },
])");
}
