#pragma once

#include "util/pprint.h"

namespace sql {

// The literal value NULL.
struct Null {
  void pprint_write(pprint::Writer &w) const { w.write("Null"); }
};

// A literal expression.
template <typename T> struct Literal {
  T value;
  void pprint_write(pprint::Writer &w) const {
    w.open_struct("Literal");
    w.field("value", this->value);
    w.close_struct();
  }
};

} // namespace sql
