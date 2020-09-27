This wants to be an in-memory column store.

But for now it's just some random code in that general direction.
What's implemented:
- a tiny bit of a SQL parser.

## Development

### Environment setup

Run `./setup` to get submodule dependencies.  
Install `clang`, `clang-format` and `scons`.

### Compiling

`scons`

### Tests

We use [`gtest`](https://github.com/google/googletest).

`./test` will both compile and run tests.

### Code formatting

`./format` will run `clang-format` all over the code (in place). Use that.
