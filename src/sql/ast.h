#pragma once

#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "sql/common.h"

namespace sql {

struct BinaryExpr;
struct ColumnRef;

// *Expression* - denotes either:
// - a scalar value (ordinary expressions)
// - many scalar values (in the special case of sometable.*; not implemented
// yet)
// - weird stuff (when it contains aggregate functions; not implemented yet)
using Expr = std::variant<ColumnRef, Literal<int>, Literal<std::string>,
                          Literal<Null>, BinaryExpr>;

// An expression which references a column from a source table.
// If `table_name` is present, such a relation must be in scope, and must have a
// column named `name`. Otherwise a column named `name` must be present in
// exactly one of the in-scope tables.
struct ColumnRef {
  std::optional<std::string> table_name;
  std::string name;
};

enum BinaryOp { EQUAL };

struct BinaryExpr {
  std::unique_ptr<Expr> lhs;
  BinaryOp op;
  std::unique_ptr<Expr> rhs;
};

struct TableRef {
  std::string table_name;
};

struct Select;

// *Relational expression* - denotes a relation.
using RelExpr = std::variant<Select, TableRef>;

// A relation with possible alias name.
//
// Constraint: If the RelExpr is a sub-select, then alias must be present.
struct RelationRef {
  std::unique_ptr<RelExpr> rel;
  std::optional<std::string> alias;
};

struct OutputColumn {
  Expr value;
  std::optional<std::string> alias;
};

struct Select {
  std::vector<OutputColumn> columns;
  std::vector<RelationRef> from;
  std::optional<Expr> where;
};

} // namespace sql
