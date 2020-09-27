#include "sql/parser.h"
#include "util/test.h"

std::variant<sql::ParseError, sql::Select>
parse_select(const std::string &input) {
  try {
    return std::variant<sql::ParseError, sql::Select>(sql::parse_select(input));
  } catch (sql::ParseError e) {
    return std::variant<sql::ParseError, sql::Select>(e);
  }
}

TEST(parser, simple_select) {
  ASSERT_SNAPSHOT_EQ(parse_select("SELECT id FROM users"),
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

TEST(parser, select_many_columns) {
  ASSERT_SNAPSHOT_EQ(parse_select("SELECT id, name, role FROM users"),
                     R"(Select {
  columns: [
    OutputColumn {
      value: ColumnRef {
        table_name: optional(),
        name: "id",
      },
      alias: optional(),
    },
    OutputColumn {
      value: ColumnRef {
        table_name: optional(),
        name: "name",
      },
      alias: optional(),
    },
    OutputColumn {
      value: ColumnRef {
        table_name: optional(),
        name: "role",
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
