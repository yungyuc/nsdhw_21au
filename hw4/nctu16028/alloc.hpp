#pragma once
#include <cstdlib>
#include <new>
#include <memory>
#include <limits>
#include <stdexcept>
#include <atomic>
#include <vector>
using namespace std;

struct ByteCounterImpl
{
    std::atomic_size_t allocated = 0;
    std::atomic_size_t deallocated = 0;
    std::atomic_size_t refcount = 0;
}; /* end struct ByteCounterImpl */

/**
 * One instance of this counter is shared among a set of allocators.
 *
 * The counter keeps track of the bytes allocated and deallocated, and report
 * those two numbers in addition to bytes that remain allocated.
 */
class ByteCounter
{
public:
    ByteCounter();                                  // default construtor
    ByteCounter(ByteCounter const & );              // copy constructor
    ByteCounter(ByteCounter       &&);              // move constructor
    ByteCounter & operator=(ByteCounter const & );  // copy assignment operator
    ByteCounter & operator=(ByteCounter       &&);  // move assignment operator
    ~ByteCounter();                                 // destructor
    void swap(ByteCounter &);                       // swap two counters
    void increase(size_t amount);                   // record the allocation
    void decrease(size_t amount);                   // record the deallocation
    size_t bytes() const;                           // getter
    size_t allocated() const;                       // getter
    size_t deallocated() const;                     // getter
    size_t refcount() const;                        // getter (for debugging)
    ByteCounterImpl *m_impl;

private:
    void incref();
    void decref();
}; /* end of class ByteCounter */

/**
 * Very simple allocator that counts the number of bytes allocated through it.
 *
 * It's made to demonstrate the STL allocator and only works in this example.
 * A lot of modification is needed to use it in a real application.
 */
template <class T>
struct CustomAllocator
{

    using value_type = T;

    // Just use the default constructor of ByteCounter for the data member "counter".
    CustomAllocator() = default;

    template <class U> constexpr
    CustomAllocator(const CustomAllocator<U> & other) noexcept
    {
        counter = other.counter;
    }

    T * allocate(std::size_t n)
    {
        if(n > std::numeric_limits<std::size_t>::max() / sizeof(T))
            throw std::bad_alloc();

        const std::size_t bytes = n*sizeof(T);
        T * p = static_cast<T *>(std::malloc(bytes));
        if(p)
        {
            counter.increase(bytes);
            return p;
        }
        else
            throw std::bad_alloc();
    }

    void deallocate(T* p, std::size_t n) noexcept
    {
        std::free(p);
        const std::size_t bytes = n * sizeof(T);
        counter.decrease(bytes);
    }

    ByteCounter counter;

}; /* end of struct CustomAllocator */

template <class T, class U>
bool operator==(const CustomAllocator<T> & a, const CustomAllocator<U> & b)
{
    //return a.counter == b.counter;
    if(a.counter.m_impl->allocated != b.counter.m_impl->allocated)
        return false;
    if(a.counter.m_impl->deallocated != b.counter.m_impl->deallocated)
        return false;
    return (a.counter.m_impl->refcount == b.counter.m_impl->refcount);
}

template <class T, class U>
bool operator!=(const CustomAllocator<T> & a, const CustomAllocator<U> & b)
{
    return !(a == b);
}

