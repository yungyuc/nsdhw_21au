#include "allocator.hpp"

ByteCountData _byte_count_data = {0, 0};

size_t bytes_allocated() noexcept { return _byte_count_data.bytes_allocated; }

size_t bytes_deallocated() noexcept {
  return _byte_count_data.bytes_deallocated;
}

size_t bytes_alive() noexcept {
  return _byte_count_data.bytes_allocated - _byte_count_data.bytes_deallocated;
}
