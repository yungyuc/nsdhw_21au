#pragma once
#ifndef NSDHW_21AU_HW4_ALLOCATOR_HPP
#define NSDHW_21AU_HW4_ALLOCATOR_HPP

#include <cstddef>
#include <memory>

struct ByteCountData {
  size_t bytes_allocated, bytes_deallocated;
} extern _byte_count_data;

size_t bytes_allocated() noexcept;
size_t bytes_deallocated() noexcept;
size_t bytes_alive() noexcept;

template <typename T, typename Alloc = std::allocator<T>>
class ByteCountAllocator {
public:
  using value_type = T;

  // Constructors & assignment operators

  ByteCountAllocator() : _alloc(Alloc()) {}

  ByteCountAllocator(const Alloc &alloc) noexcept : _alloc(alloc) {}

  ByteCountAllocator(Alloc &&alloc) noexcept : _alloc(std::move(alloc)) {}

  ByteCountAllocator(const ByteCountAllocator &other) noexcept
      : _alloc(other._alloc) {}

  ByteCountAllocator(ByteCountAllocator &&other) noexcept
      : _alloc(std::move(other._alloc)) {}

  ByteCountAllocator &operator=(const ByteCountAllocator &other) noexcept {
    if (this == &other)
      return *this;

    _alloc = other._alloc;
    return *this;
  }

  ByteCountAllocator &operator=(ByteCountAllocator &&other) noexcept {
    if (this == &other)
      return *this;

    _alloc = std::move(other._alloc);
    return *this;
  }

  // Destructor

  virtual ~ByteCountAllocator() = default;

  // Methods

  T *allocate(const size_t n) {
    const size_t new_bytes_allocated =
        _byte_count_data.bytes_allocated + n * sizeof(T);
    if (new_bytes_allocated < _byte_count_data.bytes_allocated) // Overflow
      throw std::bad_alloc();

    _byte_count_data.bytes_allocated = new_bytes_allocated;
    return _alloc.allocate(n);
  }

  void deallocate(T *const p, const size_t n) noexcept {
    // No need to perform overflow check, because bytes_deallocated <
    // bytes_allocated.
    _byte_count_data.bytes_deallocated += n * sizeof(T);
    _alloc.deallocate(p, n);
  }

  bool operator==(const ByteCountAllocator &other) const noexcept {
    return _alloc == other._alloc;
  }

  bool operator!=(const ByteCountAllocator &other) const noexcept {
    return _alloc != other._alloc;
  }

protected:
  Alloc _alloc;
};

#endif
