#include "storage/column.h"
#include "util/test.h"

#include <random>

using namespace storage;

TEST(column, basic_test_one_block) {
  Column<int> col({.block_size_in_items = 4});
  col.set_at(0, 100);
  ASSERT_EQ(col.get_at(0), 100);
}

TEST(column, sequential_insert) {
  Column<int> col({.block_size_in_items = 4});
  for (int i = 0; i < 100; i++) {
    col.set_at(i, 1000 + i);
    ASSERT_EQ(col.size(), i + 1);
  }
  for (int i = 0; i < 100; i++) {
    ASSERT_EQ(col.get_at(i), 1000 + i);
  }
}

TEST(column, random_ops) {
  Column<int> col({.block_size_in_items = 4});
  col.set_at(0, 0);

  std::vector mirror{0};
  std::minstd_rand rng;

  for (int i = 0; i < 10000; i++) {
    // Read a random value
    {
      size_t index = rng() % col.size();
      ASSERT_EQ(col.get_at(index), mirror[index]);
    }

    // Update a random value
    {
      size_t index = rng() % (col.size() + 1);
      int value = rng();
      col.set_at(index, value);
      if (index == mirror.size()) {
        mirror.push_back(value);
      } else {
        mirror[index] = value;
      }
    }
  }
}
