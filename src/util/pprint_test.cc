#include "gtest/gtest.h"

#include "util/pprint.h"

struct Foo {
  std::string bar;
  int baz;

  void pprint_write(pprint::Writer &w) const {
    w.open_struct("Foo");
    w.field("bar", this->bar);
    w.field("baz", this->baz);
    w.close_struct();
  }
};

TEST(pprint, simple_struct) {
  ASSERT_EQ(pprint::pprint(Foo{.bar = "Hello", .baz = 42}),
            "Foo { bar: \"Hello\", baz: 42, }");
}

TEST(pprint, vector) {
  ASSERT_EQ(pprint::pprint(std::vector{1, 2, 3}), "[ 1, 2, 3, ]");
}
