#include <cstring>
#include <cstddef>
#include <algorithm>
#include <vector>
#include <atomic>
#include <limits>
#include <new>
#include <mkl.h>


struct ByteCounterImpl
{

    std::atomic_size_t allocated = {0};
    std::atomic_size_t deallocated = {0};
    std::atomic_size_t refcount = {0};

};


class ByteCounter
{

public:

    ByteCounter() : m_impl(new ByteCounterImpl)
    { incref(); }

    ByteCounter(ByteCounter const & other) : m_impl(other.m_impl)
    { incref(); }

    ByteCounter & operator= (ByteCounter const & other)
    {
        if (&other != this)
        {
            decref();
            m_impl = other.m_impl;
            incref();
        }

        return *this;
    }

    ByteCounter(ByteCounter && other) : m_impl(other.m_impl)
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
            return;
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

};


template <class T>
struct MyAllocator
{

    using value_type = T;

    // Just use the default constructor of ByteCounter for the data member
    // "counter".
    MyAllocator() = default;

    template <class U> constexpr
    MyAllocator(const MyAllocator<U> & other) noexcept : counter(other.counter) {}

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

    size_t bytes() const { return counter.bytes(); }
    size_t allocated() const { return counter.allocated(); }
    size_t deallocated() const { return counter.deallocated(); }

};


static MyAllocator<double> alloc;


class Matrix 
{

public:
    
    Matrix(size_t nrow, size_t ncol): m_nrow(nrow), m_ncol(ncol), m_buffer(alloc)
    {
        reset(nrow, ncol);
    }
    
    ~Matrix() = default;

    Matrix(const Matrix &M)
    {
        m_nrow = M.m_nrow;
        m_ncol = M.m_ncol;
        reset(m_nrow, m_ncol);
        
        for (size_t i = 0; i < m_nrow; ++i)
            for (size_t j = 0; j < m_ncol; ++j)
                (*this)(i, j) = M(i, j);
    }
    
    Matrix& operator= (const Matrix& M) 
    {
        if (this == &M) { return *this; }

        if (m_nrow != M.m_nrow || m_ncol != M.m_ncol) reset(M.m_nrow, M.m_ncol);
        for (size_t i = 0; i < m_nrow; i++)
            for (size_t j = 0; j < m_ncol; j++)
                (*this)(i, j) = M(i, j);

        return *this;

    }

    bool operator== (Matrix const &M) const 
    {
        for (size_t i = 0; i < m_nrow; i++) 
            for (size_t j = 0;j < m_ncol; j++)
                if (m_buffer[i*m_ncol + j] != M(i, j)) return false;
                
        return true;
    }

    double   operator() (size_t row, size_t col) const { return m_buffer[row*m_ncol + col]; }
    double & operator() (size_t row, size_t col)       { return m_buffer[row*m_ncol + col]; }
    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    
    void reset(size_t nrow, size_t ncol) 
    {
        size_t nelement = nrow * ncol;
        m_buffer.resize(nelement, 0);
        m_nrow = nrow;
        m_ncol = ncol;
    }

    size_t m_nrow;
    size_t m_ncol;
    std::vector<double, MyAllocator<double>> m_buffer;

};


Matrix multiply_naive(Matrix const & mat1, Matrix const & mat2);
Matrix multiply_tile(Matrix const & mat1, Matrix const & mat2, size_t tsize);
Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2);


size_t bytes();
size_t allocated();
size_t deallocated();