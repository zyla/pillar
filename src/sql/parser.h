#pragma once

#include <string>

#include "sql/ast.h"
#include "sql/common.h"

namespace sql {

struct ParseError {
  size_t pos;
  std::string message;

  void pprint_write(pprint::Writer &w) const {
    w.open_struct("ParseError");
    w.field("pos", this->pos);
    w.field("message", this->message);
    w.close_struct();
  }
};

// Throws ParseError
Select parse_select(const std::string &input);

} // namespace sql
