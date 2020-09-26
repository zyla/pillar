#include "gtest/gtest.h"

#include "util/pprint.h"

struct Foo {
  std::string bar;
  int baz;
  std::vector<int> qux;

  void pprint_write(pprint::Writer &w) const {
    w.open_struct("Foo");
    w.field("bar", this->bar);
    w.field("baz", this->baz);
    w.field("qux", this->qux);
    w.close_struct();
  }
};

TEST(pprint, simple_struct) {
  ASSERT_EQ(pprint::pprint(Foo{.bar = "Hello", .baz = 42, .qux = {1, 2, 3}}),
            R"(Foo { bar: "Hello", baz: 42, qux: [ 1, 2, 3, ], })");
}

TEST(pprint, simple_struct_multiline) {
  ASSERT_EQ(pprint::pprint_multiline(
                Foo{.bar = "Hello", .baz = 42, .qux = {1, 2, 3}}),
            R"(Foo {
  bar: "Hello",
  baz: 42,
  qux: [
    1,
    2,
    3,
  ],
})");
}

TEST(pprint, variant) {
  std::variant<int, std::string> v1(42);
  ASSERT_EQ(pprint::pprint(v1), "42");

  std::variant<int, std::string> v2("foo");
  ASSERT_EQ(pprint::pprint(v2), R"("foo")");
}
