#include <string>
#include <utility>
#include <cstddef>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <atomic>



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

    ByteCounter()
      : m_impl(new ByteCounterImpl)
    { incref(); }

    ByteCounter(ByteCounter const & other)
      : m_impl(other.m_impl)
    { incref(); }

    ByteCounter & operator=(ByteCounter const & other)
    {
        if (&other != this)
        {
            decref();
            m_impl = other.m_impl;
            incref();
        }

        return *this;
    }

    ByteCounter(ByteCounter && other)
      : m_impl(other.m_impl)
    { incref(); }

    ByteCounter & operator=(ByteCounter && other)
    {
        if (&other != this)
        {
            decref();
            m_impl = other.m_impl;
            incref();
        }

        return *this;
    }

    ~ByteCounter() { decref(); }

    void swap(ByteCounter & other)
    {
        std::swap(m_impl, other.m_impl);
    }

    void increase(std::size_t amount)
    {
        m_impl->allocated += amount;
    }

    void decrease(std::size_t amount)
    {
        m_impl->deallocated += amount;
    }

    std::size_t bytes() const { return m_impl->allocated - m_impl->deallocated; }
    std::size_t allocated() const { return m_impl->allocated; }
    std::size_t deallocated() const { return m_impl->deallocated; }
    /* This is for debugging. */
    std::size_t refcount() const { return m_impl->refcount; }

private:

    void incref() { ++m_impl->refcount; }

    void decref()
    {
        if (nullptr == m_impl)
        {
            // Do nothing.
        }
        else if (1 == m_impl->refcount)
        {
            delete m_impl;
            m_impl = nullptr;
        }
        else
        {
            --m_impl->refcount;
        }
    }

    ByteCounterImpl * m_impl;

}; /* end class ByteCounter */

template <class T>
struct CustomAllocator
{

    using value_type = T;

    // Just use the default constructor of ByteCounter for the data member
    // "counter".
    CustomAllocator() = default;

    template <class U> constexpr
    CustomAllocator(const CustomAllocator<U> & other) noexcept
    {
        counter = other.counter;
    }

    T * allocate(std::size_t n)
    {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
        {
            throw std::bad_alloc();
        }
        const std::size_t bytes = n*sizeof(T);
        T * p = static_cast<T *>(std::malloc(bytes));
        if (p)
        {
            counter.increase(bytes);
            return p;
        }
        else
        {
            throw std::bad_alloc();
        }
    }

    void deallocate(T* p, std::size_t n) noexcept
    {
        std::free(p);

        const std::size_t bytes = n*sizeof(T);
        counter.decrease(bytes);
    }

    ByteCounter counter;

}; /* end struct CustomAllocator */


static CustomAllocator<double> allocator;

class Matrix {

public:

    Matrix() : m_nrow(0), m_ncol(0), m_elems(allocator) {} // default constructor 
    // Matrix(Matrix &&);                                // move constructor
    // Matrix & operator=(Matrix &&);                    // move assignment operator
    Matrix(size_t r, size_t c) : m_nrow(r), m_ncol(c), m_elems(allocator) {  // custom constructor
        if (r == 0 || c == 0) {
            throw std::out_of_range("matrix index out of range");
        }
        m_elems.resize(r * c, 0.0);
    }


    Matrix(const Matrix &rhs) : m_elems(allocator) { // copy constructor
        m_nrow = rhs.m_nrow;
        m_ncol = rhs.m_ncol;

        m_elems.resize(m_nrow * m_ncol, 0.0);

        for (int i = 0; i < m_nrow * m_ncol; i++) {
            m_elems[i] = rhs.m_elems[i]; 
        }
    }


    Matrix(Matrix &&rhs) : m_nrow(rhs.m_nrow), m_ncol(rhs.m_ncol), m_elems(allocator) {
        m_elems.resize(0, 0.0);
        std::swap(m_nrow, rhs.m_nrow);
        std::swap(m_ncol, rhs.m_ncol);
        std::swap(m_elems, rhs.m_elems);
    }

    double& operator() (size_t i, size_t j) {
        /*
            Remove this check to speed up the performance
            if (i >= m_nrow || j >= m_ncol)
                throw std::out_of_range("matrix index out of range");
        */
        return m_elems[i * m_ncol + j];
    }

    double operator() (size_t i, size_t j) const {
        /*
            Remove this check to speed up the performance
            if (i >= m_nrow || j >= m_ncol)
                throw std::out_of_range("matrix index out of range");
        */
        return m_elems[i * m_ncol + j];
    }


    std::pair<size_t, size_t> size() const;
    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_nrow; }

    bool operator== (const Matrix &rhs) const; 

    std::string to_string() const;

    /* For accessing the inner buffer */
    friend Matrix multiply_mkl(const Matrix &A, const Matrix &B);

private:
    bool check_range(size_t i, size_t j) const;
    size_t m_nrow, m_ncol;
    std::vector<double, CustomAllocator<double>> m_elems;

};


size_t bytes() { return allocator.counter.bytes(); }
size_t allocated() { return allocator.counter.allocated(); }
size_t deallocated() { return allocator.counter.deallocated(); }

Matrix multiply_naive(const Matrix &A, const Matrix &B);
Matrix multiply_tile(const Matrix &A, const Matrix &B, size_t tile_width); 
Matrix multiply_mkl(const Matrix &A, const Matrix &B);
