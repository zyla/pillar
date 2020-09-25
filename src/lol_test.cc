#include "lol.h"
#include "gtest/gtest.h"

TEST(LolTest, Add) { ASSERT_EQ(lol::add(1, 2), 3); }
