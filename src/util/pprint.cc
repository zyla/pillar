#include "util/pprint.h"

namespace pprint {

template <> void pprint_write(Writer &w, const std::string &v) {
  w.out << "\"" << v << "\""; // FIXME: escaping
}

template <> void pprint_write(Writer &w, const int &v) { w.out << v; }

} // namespace pprint
