#include "sql/parser.h"
#include "sql/ast.h"
#include "sql/lexer.h"

#include <optional>
#include <variant>

namespace sql {

using lexer::LocatedToken;
using lexer::Token;
using lexer::TokenType;

struct Parser {
  const std::vector<LocatedToken> &tokens;
  size_t pos = 0;

  bool eof() { return pos >= tokens.size(); }

  ParseError parse_error(const std::string message) {
    return ParseError{.pos = this->pos, .message = message};
  }

  const Token &match(TokenType type) {
    if (this->eof()) {
      throw this->parse_error("EOF");
    }
    if (this->tokens[this->pos].token.token_type != type) {
      throw this->parse_error("Unexpected token: " +
                              pprint::pprint(this->tokens[this->pos].token));
    }
    const auto &token = this->tokens[this->pos].token;
    this->pos++;
    return token;
  }

  // Parse a given production.
  // If it fails at lookahead (i.e. without consuming any input), return None.
  // If it fails after consuming some input, rethrow the error.
  template <typename T, typename Parse> std::optional<T> try_parse(Parse p) {
    size_t start_pos = this->pos;
    try {
      return std::optional<T>(p());
    } catch (ParseError) {
      if (this->pos == start_pos) {
        return std::optional<T>();
      } else {
        throw;
      }
    }
  }

  // Parse a given production.
  // If it fails at lookahead (i.e. without consuming any input), return false.
  // If it fails after consuming some input, rethrow the error.
  template <typename Parse> bool try_parse_(Parse p) {
    size_t start_pos = this->pos;
    try {
      p();
      return true;
    } catch (ParseError) {
      if (this->pos == start_pos) {
        return false;
      } else {
        throw;
      }
    }
  }

  template <typename T, typename ParseSep, typename ParseT>
  std::vector<T> sep_by_1(ParseSep parse_sep, ParseT parse_t) {
    std::vector<T> result;
    result.push_back(parse_t());

    while (this->try_parse_(parse_sep)) {
      result.push_back(parse_t());
    }
    return result;
  }

  OutputColumn output_column() {
    auto expr = this->expr();
    return OutputColumn{.value = std::move(expr),
                        .alias = std::optional<std::string>()};
  }

  Select select() {
    this->match(TokenType::Select);

    std::vector<OutputColumn> columns = this->sep_by_1<OutputColumn>(
        [this] { this->match(TokenType::Comma); },
        [this]() -> OutputColumn { return this->output_column(); });

    this->match(TokenType::From);

    // For now just parse a single table
    auto table_name = this->identifier();
    std::vector<RelationRef> from;
    from.push_back(RelationRef{
        .rel = std::make_unique<RelExpr>(TableRef{table_name}),
        .alias = std::optional<std::string>(),
    });

    return Select{
        .columns = std::move(columns),
        .from = std::move(from),
        .where = std::optional<Expr>(),
    };
  }

  std::string identifier() {
    auto token = this->match(TokenType::Identifier);
    return std::get<std::string>(token.value);
  }

  Expr expr() {
    auto name = this->identifier();
    return Expr(ColumnRef{
        .table_name = std::optional<std::string>(),
        .name = name,
    });
  }
};

Select parse_select(const std::string &input) {
  auto tokens = lexer::lex(input);
  Parser parser{tokens};
  return parser.select();
}

} // namespace sql
