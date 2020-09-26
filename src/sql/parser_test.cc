#include "sql/parser.h"
#include "util/test.h"

TEST(parser, simple_select) {
  ASSERT_SNAPSHOT_EQ(sql::parse_select("SELECT id FROM users"),
                     R"(Select {
  columns: [
    OutputColumn {
      value: ColumnRef {
        table_name: optional(),
        name: "id",
      },
      alias: optional(),
    },
  ],
  from: [
    RelationRef {
      rel: TableRef {
        table_name: "users",
      },
      alias: optional(),
    },
  ],
  where: optional(),
})");
}
