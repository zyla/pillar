#include "gtest/gtest.h"

#define ASSERT_SNAPSHOT_EQ(value, output)                                      \
  do {                                                                         \
    auto result = pprint::pprint_multiline(value);                             \
    EXPECT_EQ(result, output)                                                  \
        << "To update snapshot, copypaste this:\n                     R\"("    \
        << result << ")\");";                                                  \
  } while (0)
