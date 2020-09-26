#pragma once

#include <string>

#include "sql/ast.h"
#include "sql/common.h"

namespace sql {

struct ParseError {
  size_t pos;
  std::string message;
};

// Throws ParseError
Select parse_select(const std::string &input);

} // namespace sql
