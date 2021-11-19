#include <iostream>
#include <vector>
#include <mkl.h>
#include <pybind11/pybind11.h>
namespace py = pybind11;

struct ByteCounterImpl
{

    std::size_t allocated = 0;
    std::size_t deallocated = 0;
    std::size_t refcount = 0;

};

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

    ByteCounter(ByteCounter && other) noexcept
            : m_impl(other.m_impl)
    { incref(); }

    ByteCounter & operator=(ByteCounter && other) noexcept
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

};

template <class T>
struct CustomAllocator
{

    using value_type = T;

    // Just use the default constructor of ByteCounter for the data member
    // "counter".
    CustomAllocator() = default;

    template <class U> constexpr
    explicit CustomAllocator(const CustomAllocator<U> & other) noexcept
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

};

CustomAllocator<double> alloc;
struct Matrix {

public:

    Matrix(size_t nrow, size_t ncol)
            : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
    }

    Matrix(Matrix const & other)
            : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
        reset_buffer(other.m_nrow, other.m_ncol);
        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                (*this)(i,j) = other(i,j);
            }
        }
    }

    Matrix & operator=(Matrix const & other)
    {
        if (this == &other) { return *this; }
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol)
        {
            reset_buffer(other.m_nrow, other.m_ncol);
        }
        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                (*this)(i,j) = other(i,j);
            }
        }
        return *this;
    }

    Matrix(Matrix && other) noexcept
            : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
    }

    Matrix & operator=(Matrix && other) noexcept
    {
        if (this == &other) { return *this; }
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
        return *this;
    }

    ~Matrix() {
        std::vector<double, CustomAllocator<double>>(alloc).swap(m_buffer);
    }

    bool operator==(Matrix const & other) {
        if(nrow() != other.nrow() || ncol() != other.ncol())
            return false;
        for(size_t i=0; i<size(); ++i)
            if((*this)[i] != other[i])
                return false;
        return true;
    }

    double   operator() (size_t row, size_t col) const { return m_buffer.at(index(row, col)); }
    double & operator() (size_t row, size_t col)       { return m_buffer.at(index(row, col)); }

    double   operator[] (size_t idx) const { return m_buffer.at(idx); }
    double & operator[] (size_t idx)       { return m_buffer.at(idx); }

    size_t size() const { return m_nrow * m_ncol; }
    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    Matrix transpose() const;

public:

    size_t index(size_t row, size_t col) const
    {
        return row * m_ncol + col;
    }

    void reset_buffer(size_t nrow, size_t ncol)
    {
        const size_t nelement = nrow * ncol;
        if (nelement) {
            std::vector<double, CustomAllocator<double>>(nelement, alloc).swap(m_buffer);
        }
        m_nrow = nrow;
        m_ncol = ncol;
    }

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    std::vector<double, CustomAllocator<double>> m_buffer;
};

Matrix Matrix::transpose() const
{
    Matrix ret(nrow(), ncol());

    for (size_t i=0; i<ret.nrow(); ++i)
    {
        for (size_t j=0; j<ret.ncol(); ++j)
        {
            ret(j, i) = (*this)(i, j);
        }
    }

    return ret;
}

void initialize(Matrix & mat)
{
    for (size_t i=0; i<mat.nrow(); ++i)
    {
        for (size_t j=0; j<mat.ncol(); ++j)
        {
            if (0 == i%2)
            {
                mat(i, j) = (double)j;
            }
            else
            {
                mat(i, j) = (double)(mat.ncol() - 1 - j);
            }
        }
    }
}

Matrix multiply_naive(const Matrix & mat1, const Matrix & mat2) {
    if (mat1.ncol() != mat2.nrow())
        throw std::out_of_range("k not eq");
    Matrix ret(mat1.nrow(), mat2.ncol());
    for (size_t i=0; i<mat1.nrow(); ++i)
    {
        for (size_t k=0; k<mat2.ncol(); ++k)
        {
            double v = 0;
            for (size_t j=0; j<mat1.ncol(); ++j)
            {
                v += mat1(i,j) * mat2(j,k);
            }
            ret(i,k) = v;
        }
    }
    return ret;
}

Matrix multiply_tile(const Matrix & mat1, const Matrix & mat2, int width) {
    if (mat1.ncol() != mat2.nrow())
        throw std::out_of_range("k not eq");
    size_t R = mat1.nrow();
    size_t C = mat2.ncol();
    size_t K = mat1.ncol();
    Matrix mat2T = mat2.transpose();
    Matrix ret(R, C);

    for(size_t r=0; r<R; ++r)
        for(size_t c=0; c<C; ++c)
            ret(r, c) = 0.0;

    for(size_t r=0; r<R; r+=width) {                    // block row 開頭
        size_t r_end = r+width > R ? R : r+width;     // block row 結尾 (定義出一整列的 blocks)
        for(size_t c=0; c<C; c+=width) {                // block col 開頭
            size_t c_end = c+width > C ? C : c+width; // block col 結尾 (定義出一整行的 blocks)
            for(size_t k=0; k<K; k+=width) {             // 兩個 block 要做 multiplication
                size_t k_end = k+width > K ? K : k+width;

                for(size_t cp=c; cp<c_end; ++cp) {
                    size_t sj = C * cp;
                    for(size_t rp=r; rp<r_end; ++rp) {
                        size_t ki = K * rp;
                        size_t kij = ki + cp;
                        for(size_t kp=k; kp<k_end; ++kp)
                            ret[kij] += mat1[ki + kp] * mat2T[sj + kp];
                    }
                }
//                for(size_t rp=r; rp<r_end; ++rp)
//                    for(size_t cp=c; cp<c_end; ++cp) {
//                        for(size_t kp=k; kp<k_end; ++kp)
//                            ret(rp, cp) += mat1(rp, kp) * mat2(kp, cp);
//                    }
            }
        }
    }
    return ret;
}

Matrix multiply_mkl(const Matrix & mat1, const Matrix & mat2) {
    Matrix ret(mat1.nrow(), mat2.ncol());
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                (int)mat1.nrow(), (int)mat2.ncol(), (int)mat1.ncol(), 1.0,
                mat1.m_buffer.data(), (int)mat1.ncol(),
                mat2.m_buffer.data(), (int)mat2.ncol(), 0.0,
                ret.m_buffer.data(), (int)ret.ncol());
    return ret;
}

std::size_t bytes() {
    return alloc.counter.bytes();
}

std::size_t allocated() {
    return alloc.counter.allocated();
}

std::size_t  deallocated() {
    return alloc.counter.deallocated();
}

int main() {
    Matrix mat1(1 * 1024, 1 * 1024);
    initialize(mat1);
    Matrix mat2 = mat1;
    Matrix naive_ret = multiply_naive(mat1, mat2);
    Matrix tile_ret = multiply_tile(mat1, mat2, 64/sizeof(float));
    Matrix mkl_ret = multiply_mkl(mat1, mat2);
    std::cout << (naive_ret==tile_ret) << std::endl;
    std::cout << (tile_ret==mkl_ret) << std::endl;

    return 0;
}

PYBIND11_MODULE(_matrix, m) {
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);
    m.def("bytes", &bytes);
    m.def("allocated", &allocated);
    m.def("deallocated", &deallocated);
    py::class_<Matrix>(m, "Matrix")
            .def(py::init<int, int>())
            .def_property_readonly("nrow", &Matrix::nrow)
            .def_property_readonly("ncol", &Matrix::ncol)
            .def("__eq__",  &Matrix::operator==)
            .def("__getitem__", [](Matrix & self, std::pair<size_t, size_t> pair) { return self(pair.first, pair.second); })
            .def("__setitem__", [](Matrix & self, std::pair<size_t, size_t> pair, double item) { self(pair.first, pair.second) = item; });
}
