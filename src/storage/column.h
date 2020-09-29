#pragma once

#include "util/pprint.h"

#include <memory>
#include <vector>

// A Column<T> is a big array of T.
// Records are indexed using logical identifiers in a flat identifier space.

namespace storage {

using pprint::pprint;

struct Settings {
  size_t block_size_in_items;
};

template <typename T> class Column {
private:
  struct BlockMapping {
    size_t start_index;
    size_t size;
    std::unique_ptr<T[]> data;
  };

public:
  Column(Settings settings) : settings(settings) {}

  const T &get_at(size_t index) {
    if (index >= this->size_) {
      throw std::out_of_range(
          "index " + pprint(index) +
          " out of range for get_at. size=" + pprint(this->size_));
    }
    const auto &block = this->find_block(index, NoCreate);
    return block.data[index - block.start_index];
  }

  /**
   * Sets a value at the given index.
   * The index can be the current size, in which case the array will be
   * expanded.
   */
  void set_at(size_t index, T value) {
    if (index > this->size_) {
      throw std::out_of_range(
          "index " + pprint(index) +
          " out of range for set_at. size=" + pprint(this->size_));
    }
    const auto &block = this->find_block(index, Create);
    block.data[index - block.start_index] = value;
    if (index == this->size_) {
      this->size_ = index + 1;
    }
  }

  size_t size() const { return this->size_; }

private:
  size_t size_ = 0;
  std::vector<BlockMapping> block_map;
  Settings settings;

  enum CreateBlock {
    NoCreate = 0,
    Create = 1,
  };

  BlockMapping &find_block(size_t index, CreateBlock create) {
    size_t max_size = 0;
    for (auto &block : this->block_map) {
      if (block.start_index <= index &&
          index < block.start_index + block.size) {
        return block;
      }
      max_size = std::max(max_size, block.start_index + block.size);
    }
    if (create == Create) {
      auto size = this->settings.block_size_in_items;
      std::unique_ptr<T[]> data(new T[size]);
      this->block_map.push_back(BlockMapping{
          .start_index = max_size, .size = size, .data = std::move(data)});
      return this->block_map[this->block_map.size() - 1];
    }
    throw std::out_of_range("unable to find block for index " +
                            pprint::pprint(index));
  }
};

} // namespace storage
