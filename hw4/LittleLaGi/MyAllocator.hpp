#ifndef __MYALLOCATOR_HPP
#define __MYALLOCATOR_HPP

#include <atomic>
#include <limits>
#include <new>
#include <cstdlib>

struct ByteCounterImpl {
    std::atomic_size_t allocated = 0;
    std::atomic_size_t deallocated = 0;
    std::atomic_size_t refcount = 0;
};

class ByteCounter {
  private:
	ByteCounterImpl *m_counter;

    void incref() {
        ++m_counter->refcount;
    }

    void decref() {
        if (m_counter == nullptr) {
            return;
        }
        
        if (m_counter->refcount == 1) {
            delete m_counter;
            m_counter = nullptr;
        } else {
            --m_counter->refcount;
        }
    }
  public:
    ~ByteCounter() { decref(); }

    ByteCounter() : m_counter(new ByteCounterImpl) {
        incref();
    }

    ByteCounter(const ByteCounter &other) : m_counter(other.m_counter) {
        incref();
    }

    void swap(ByteCounter &other) {
        std::swap(m_counter, other.m_counter);
    }

    void increase(std::size_t amount) {
        m_counter->allocated += amount;
    }

    void decrease(std::size_t amount) {
        m_counter->deallocated += amount;
    }

    std::size_t bytes() const { return m_counter->allocated - m_counter->deallocated; }
    std::size_t allocated() const { return m_counter->allocated; }
    std::size_t deallocated() const { return m_counter->deallocated; }
};

template <class T>
struct MyAllocator {

    using value_type = T;

    // Just use the default constructor of ByteCounter for the data member
    // "m_counter".
    MyAllocator() = default;

    template <class U> constexpr
    MyAllocator(const MyAllocator<U> & other) noexcept : m_counter(other.m_counter) {}

    T *allocate(std::size_t n) {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T)) {
            throw std::bad_alloc();
        }

        const std::size_t bytes = n * sizeof(T);
        T *p = static_cast<T *>(std::malloc(bytes));
        if (p) {
            m_counter.increase(bytes);
            return p;
        } else {
            throw std::bad_alloc();
        }
    }

    void deallocate(T *p, std::size_t n) noexcept {
        std::free(p);

        const std::size_t bytes = n * sizeof(T);
        m_counter.decrease(bytes);
    }

    std::size_t bytes() const { return m_counter.bytes(); }
    std::size_t allocated() const { return m_counter.allocated(); }
    std::size_t deallocated() const { return m_counter.deallocated(); }

    ByteCounter m_counter;
};

#endif