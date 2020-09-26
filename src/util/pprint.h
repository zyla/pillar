#pragma once

#include <optional>
#include <ostream>
#include <sstream>
#include <variant>
#include <vector>

namespace pprint {

struct Writer;

template <typename T> void pprint_write(Writer &w, const T &v) {
  v.pprint_write(w);
}

struct Writer {
  std::ostream &out;
  bool with_newlines;
  int indent;

  void write_indent() {
    if (!this->with_newlines) {
      return;
    }
    for (int i = 0; i < this->indent; i++) {
      this->out << "  ";
    }
  }

  void write_newline() {
    if (this->with_newlines) {
      this->out << "\n";
    } else {
      this->out << " ";
    }
  }

  void open_struct(const std::string &name) {
    this->out << name << " {";
    this->write_newline();
    this->indent++;
  }

  void close_struct() {
    this->indent--;
    this->write_indent();
    this->out << "}";
  }

  template <typename T> void field(const std::string &name, const T &value) {
    this->write_indent();
    this->out << name << ": ";
    pprint_write(*this, value);
    this->out << ",";
    this->write_newline();
  }

  void write(const std::string &s) { this->out << s; }
};

template <> void pprint_write(Writer &w, const std::string &v);
template <> void pprint_write(Writer &w, const int &v);
template <> inline void pprint_write(Writer &w, const size_t &v) {
  w.out << v;
};

template <typename T>
inline void pprint_write(Writer &w, const std::vector<T> &v) {
  w.write("[");
  w.write_newline();
  w.indent++;
  for (const auto &x : v) {
    w.write_indent();
    pprint_write(w, x);
    w.write(",");
    w.write_newline();
  }
  w.indent--;
  w.write_indent();
  w.write("]");
}

template <typename T>
inline void pprint_write(Writer &w, const std::variant<T> &v) {
  pprint_write(w, std::get<T>(v));
}

struct PrintVisitor {
  Writer &w;

  template <typename T> void operator()(const T &v) { pprint_write(w, v); }
};

template <typename... Types>
inline void pprint_write(Writer &w, const std::variant<Types...> &v) {
  std::visit(PrintVisitor{w}, v);
}

template <typename T>
inline void pprint_write(Writer &w, const std::optional<T> &v) {
  if (v) {
    w.write("optional(");
    pprint_write(w, v.value());
    w.write(")");
  } else {
    w.write("optional()");
  }
}

struct raw_string {
  const char *s;

  raw_string(const char *s) : s(s) {}

  void pprint_write(pprint::Writer &w) const { w.write(this->s); }
};

template <typename T> std::string pprint(const T &v) {
  std::ostringstream out;
  pprint::Writer w{out, false};
  pprint_write(w, v);
  return out.str();
}

template <typename T> std::string pprint_multiline(const T &v) {
  std::ostringstream out;
  pprint::Writer w{out, true};
  pprint_write(w, v);
  return out.str();
}

} // namespace pprint
