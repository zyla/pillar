## Assumptions

- Column store
- We're exploring query planning and execution, so:
  - Executing single query at a time
    - Can still use parallelism internally
  - No transactions

## Example query plans

Note:
- DECIMAL4(s) = DECIMAL(9, s) - in 32 bits we can store 9 decimal digits
- DECIMAL8(s) = DECIMAL(18, s) - in 64 bits we can store 18 decimal digits

Schema:

```
CREATE TABLE products (
  id BIGINT NOT NULL PRIMARY KEY,
  name TEXT NOT NULL,
  price DECIMAL4(2) NOT NULL,
);

CREATE TABLE orders (
  id BIGINT NOT NULL PRIMARY KEY,
  created_at UTCTIMESTAMP NOT NULL,
  total DECIMAL4(2) NOT NULL,
  total_vat DECIMAL4(2) NOT NULL,
);

CREATE TABLE line_items (
  id BIGINT NOT NULL PRIMARY KEY,
  order_id BIGINT,
  product_id BIGINT,
  quantity INTEGER,
  price DECIMAL4(2),
);
```

### Query 1

```
SELECT id, price FROM products;
```

```
id_iterator = products.id.iter();
price_iterator = products.price.iter();
for(i = 0; i < num_rows(products); i++) {
  output[i] = (id_iterator.next(), price_iterator.next());
}
```

### Query 2

```
SELECT id, total
FROM orders
WHERE created_at > '2020-01-01 00:00:00Z'
ORDER BY total
```

```
bitmap = eval_predicate(created_at, $0 > '2020-01-01 00:00:00Z')
filtered_total = filter(bitmap, total)
indices = sort(filtered_total)
filtered_id = filter(bitmap, id)
return (
  shuffle(indices, filtered_id),
  shuffle(indices, filtered_total)
)
```

# Sketch of major stuff

```
using StringPool = vector<string>;

enum Datatype {
  INT8,
  INT64,
  STRING,
};

class Column {
  Datatype datatype();
  Iterator iterator();

  StringPool string_pool();
};

/// Which union member is active is determined by the parent column's `datatype`.
union Datum {
  int8_t int8_;
  int64_t int64_;
  size_t string_index;
};

class Iterator {
  Datum next();
};
```

```
int compare(Datum x, Datum y);
```

```
/// Returns the permutation which sorts the data by first column.
/// All input columns must be aligned.
vector<Index> sort(vector<Column>) {
}

/// Reorders values in a column according to a permutation.
/// Pipeline breaker.
Column shuffle(vector<Index>, Column) {
}

/// Returns a column with values only where bitmap has value 1
Column<T> filter(bitmap, Column<T>) {
}
```
