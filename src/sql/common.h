#pragma once

namespace sql {

// The literal value NULL.
struct Null {};

// A literal expression.
template <typename T> struct Literal { T value; };

} // namespace sql
