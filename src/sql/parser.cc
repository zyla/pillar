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
      throw this->parse_error("Unexpected token");
    }
    const auto &token = this->tokens[this->pos].token;
    this->pos++;
    return token;
  }

  Select select() {
    this->match(TokenType::Select);

    // For now just parse a single column
    auto expr = this->expr();
    std::vector<OutputColumn> columns;
    columns.push_back(OutputColumn{.value = std::move(expr),
                                   .alias = std::optional<std::string>()});

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
