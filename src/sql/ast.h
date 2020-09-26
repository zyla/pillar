#pragma once

#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "sql/common.h"
#include "util/pprint.h"

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

  void pprint_write(pprint::Writer &w) const {
    w.open_struct("ColumnRef");
    w.field("table_name", this->table_name);
    w.field("name", this->name);
    w.close_struct();
  }
};

enum BinaryOp { EQUAL };

struct BinaryExpr {
  std::unique_ptr<Expr> lhs;
  BinaryOp op;
  std::unique_ptr<Expr> rhs;

  void pprint_write(pprint::Writer &w) const {
    w.open_struct("BinaryExpr");
    w.field("lhs", this->lhs);
    w.field("op", int(this->op)); // TODO: pretty-print the op
    w.field("rhs", this->rhs);
    w.close_struct();
  }
};

struct TableRef {
  std::string table_name;

  void pprint_write(pprint::Writer &w) const {
    w.open_struct("TableRef");
    w.field("table_name", this->table_name);
    w.close_struct();
  }
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

  void pprint_write(pprint::Writer &w) const {
    w.open_struct("RelationRef");
    w.field("rel", this->rel);
    w.field("alias", this->alias);
    w.close_struct();
  }
};

struct OutputColumn {
  Expr value;
  std::optional<std::string> alias;

  void pprint_write(pprint::Writer &w) const {
    w.open_struct("OutputColumn");
    w.field("value", this->value);
    w.field("alias", this->alias);
    w.close_struct();
  }
};

struct Select {
  std::vector<OutputColumn> columns;
  std::vector<RelationRef> from;
  std::optional<Expr> where;

  void pprint_write(pprint::Writer &w) const {
    w.open_struct("Select");
    w.field("columns", this->columns);
    w.field("from", this->from);
    w.field("where", this->where);
    w.close_struct();
  }
};

} // namespace sql
