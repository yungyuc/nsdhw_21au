#include <pybind11/pybind11.h>
#include <iostream>
#include <math.h>
#include <mkl.h>
#include <vector>
#include <limits>
#include <atomic>
// #include <mkl_cblas.h>
// #include <mkl_blas.h>
// #include <mkl_lapack.h>
// #include <mkl_lapacke.h> 
namespace py = pybind11;
class Matrix;
class Block;
// class MyAllocator;


	

struct ByteCounterImpl
{
    ByteCounterImpl()
        :allocated(0), deallocated(0), refcount(0)
        {}
    std::atomic_size_t allocated;
    std::atomic_size_t deallocated;
    std::atomic_size_t refcount;

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

    ByteCounter(): m_impl(new ByteCounterImpl){ incref(); }
    ByteCounter(ByteCounter const & other): m_impl(other.m_impl){ incref(); }
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

    ByteCounter(ByteCounter && other): m_impl(other.m_impl){ incref(); }

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
    void swap(ByteCounter & other) {std::swap(m_impl, other.m_impl); }
    void increase(std::size_t amount) { m_impl->allocated += amount; }
    void decrease(std::size_t amount) { m_impl->deallocated += amount; }
    const ByteCounterImpl* get_counter_info() const { return m_impl; }

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


template<class T>
class MyAllocator {
public:
    using value_type = T;

    // Just use the default constructor of ByteCounter for the data member
    // "counter".
    MyAllocator() = default;

    template <class U>
    MyAllocator(const MyAllocator<U> & other) noexcept
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

template <class T, class U>
bool operator==(const MyAllocator<T> & a, const MyAllocator<U> & b)
{
    return a.counter.get_counter_info() == b.counter.get_counter_info();
}

template <class T, class U>
bool operator!=(const MyAllocator<T> & a, const MyAllocator<U> & b)
{
    return !(a == b);
}

const MyAllocator<double> GlobalAllocator;

class Block {
public:
    Block(size_t nrow, size_t ncol, bool colmajor):
        m_nrow(nrow), m_ncol(ncol), m_buffer(nrow*ncol), m_colmajor(colmajor)
    {}
    
    Block(const Block &block):
        m_nrow(block.m_nrow), m_ncol(block.m_ncol), m_buffer(m_nrow*m_ncol), m_colmajor(block.m_colmajor)
    {
        for(int i = 0; i < block.m_nrow*block.m_ncol; i++) {
            m_buffer[i]=block.m_buffer[i];
        }    
    }

    ~Block() {}
    double   operator() (size_t row, size_t col) const { // for getitem
        // if (row > m_nrow) throw std::invalid_argument( "received row exceed nrow" );
        // if (col > m_ncol) throw std::invalid_argument( "received col exceed ncol" );
        if (m_colmajor)
            return m_buffer[col * m_nrow + row];
        else
            return m_buffer[row * m_ncol + col];
    }

    void setContent(const std::vector<double, MyAllocator<double>> &mat, size_t start_idx, size_t parent_matrix_ncol) {
        for (int i = 0; i < m_nrow; i++) {
            for (int j = 0; j < m_ncol; j++) {
                double tmp =  mat[start_idx + i * parent_matrix_ncol + j];
                if (m_colmajor) {
                    m_buffer[j * m_nrow + i] = tmp; //mat(start_row+i,start_col+j);
                } else {
                    m_buffer[i * m_ncol + j] = tmp; //mat(start_row+i,start_col+j);
                }
            }
        }
    }

    size_t nrow() const {return m_nrow;}
    size_t ncol() const {return m_ncol;}

private:
    bool m_colmajor;
    // double *m_buffer;
    std::vector<double> m_buffer;
    size_t m_nrow;
    size_t m_ncol;
};


class Matrix {
public:
    Matrix(size_t nrow, size_t ncol)
        :m_nrow(nrow), m_ncol(ncol), m_buffer(nrow * ncol, GlobalAllocator)
    {}
    
    Matrix(size_t nrow, size_t ncol, bool customAllocator)
        :m_nrow(nrow), m_ncol(ncol)
    {
        if (customAllocator) {
            m_buffer = std::vector<double,MyAllocator<double>>(nrow * ncol, GlobalAllocator);
        } else {
            m_buffer = std::vector<double, MyAllocator<double>>(nrow * ncol);
        }
    }

    Matrix(const Matrix &target)
        :m_nrow(target.m_nrow), m_ncol(target.m_ncol), m_buffer(target.m_nrow * target.m_ncol, GlobalAllocator) {
        for(int i =0; i < target.m_nrow*target.m_ncol; i++) {
            m_buffer[i]=target.m_buffer[i];
        } 
        // memcpy(m_buffer, target.m_buffer, sizeof(double) * row * col);
    }

    ~Matrix() {}

    // No bound check.
    double   operator() (size_t row, size_t col) const { // for getitem
        // if (row > m_nrow) throw std::invalid_argument( "received row exceed nrow" );
        // if (col > m_ncol) throw std::invalid_argument( "received col exceed ncol" );
        return m_buffer[row*m_ncol + col];
    }
    double & operator() (size_t row, size_t col) {       // for setitem
        // if (row > m_nrow) throw std::invalid_argument( "received row exceed nrow" );
        // if (col > m_ncol) throw std::invalid_argument( "received col exceed ncol" );
        return m_buffer[row*m_ncol + col];
    }
    Matrix operator+(const Matrix &mat) const {
        // if (m_nrow != mat.m_nrow || m_ncol != mat.m_ncol) throw std::invalid_argument( "received matrix shape isn't consistent" );
        Matrix result(mat);
        for (int i=0; i< m_nrow; i+=1) {
            for (int j=0; j<m_ncol; j+=1) {
                result.m_buffer[i*m_ncol+j]+=(*this)(i,j);
            }
        }
        return result;
    }
    void operator+=(const Matrix &mat) {
        // if (m_nrow != mat.m_nrow || m_ncol != mat.m_ncol) throw std::invalid_argument( "received matrix shape isn't consistent" );
        for (int i=0; i< m_nrow; i+=1) {
            for (int j=0; j<m_ncol; j+=1) {
                m_buffer[i*m_ncol+j]+=mat(i,j);
            }
        }
    }
    Matrix operator-(const Matrix &mat) const {
        // if (m_nrow != mat.m_nrow || m_ncol != mat.m_ncol) throw std::invalid_argument( "received matrix shape isn't consistent" );
        Matrix result(mat);
        for (int i=0; i< m_nrow; i+=1) {
            for (int j=0; j<m_ncol; j+=1) {
                result.m_buffer[i*m_ncol+j]-=(*this)(i,j);
            }
        }
        return result;
    }
    void operator-=(const Matrix &mat) {
        // if (m_nrow != mat.m_nrow || m_ncol != mat.m_ncol) throw std::invalid_argument( "received matrix shape isn't consistent" );
        for (int i=0; i< m_nrow; i+=1) {
            for (int j=0; j<m_ncol; j+=1) {
                m_buffer[i*m_ncol+j]-=mat(i,j);
            }
        }
    }
    void operator=(const Matrix &target) {
        m_nrow=target.m_nrow;
        m_ncol=target.m_ncol;

        m_buffer = std::vector<double, MyAllocator<double>>(target.m_ncol*target.m_nrow, GlobalAllocator);
        for (int i =0; i < target.m_nrow*target.m_ncol;i++) {
            m_buffer[i]=target.m_buffer[i]; 
        }
        // memcpy(m_buffer, target.m_buffer, sizeof(double) * row * col);
    }
    bool operator==(const Matrix &target) const{
        if (m_nrow != target.m_nrow || m_ncol != target.m_ncol) {
            return false;
        } else {
            for (int i = 0; i < m_nrow; i++) {
                for (int j = 0; j < m_ncol; j++) {
                    if ((*this)(i,j) != target(i,j)) return false;
                }
            }
            return true;
        }
    }

    Block get_block(size_t block_size, size_t row_idx, size_t col_idx, bool col2row = false) const{
        // row_idx: row index of the block
        // col_idx: col index of the block
        // if (block_size*row_idx > m_nrow) throw std::invalid_argument( "received block_size*row exceed nrow" );
        // if (block_size*col_idx > m_ncol) throw std::invalid_argument( "received block_size*col exceed ncol" );
        size_t bk_col = m_ncol - block_size*col_idx < block_size ? m_ncol - block_size*col_idx : block_size;
        size_t bk_row = m_nrow - block_size*row_idx < block_size ? m_nrow - block_size*row_idx : block_size;
        Block block(bk_row, bk_col, col2row);

        size_t target_row=(block_size*row_idx)*m_ncol;
        size_t target_col=(block_size*col_idx);
        size_t start_idx = target_row + target_col;
        block.setContent(m_buffer, start_idx, m_ncol);
        return block;
    }

    void set_block(size_t block_size, size_t row_idx, size_t col_idx, const Matrix &mat) {
        // row_idx: row index of the block
        // col_idx: col index of the block
        // if (block_size*row_idx > m_nrow || block_size*col_idx > m_ncol) throw std::invalid_argument( "received block_size*row exceed nrow or block_size*col exceed ncol" );
        size_t bk_col = m_ncol - block_size*col_idx < block_size ? m_ncol - block_size*col_idx : block_size;
        size_t bk_row = m_nrow - block_size*row_idx < block_size ? m_nrow - block_size*row_idx : block_size;
        // if (bk_row != mat.m_nrow || bk_col != mat.m_ncol) throw std::invalid_argument( "received block_shape and mat are not consistent" );

        for (int i=0;i<bk_row; i++) {
            size_t target_row=(block_size*row_idx+i)*m_ncol;
            size_t target_col=(block_size*col_idx);
            size_t start_idx=target_row+target_col;
            for (int j=0;j<bk_col;j++) {
                m_buffer[start_idx + j]=mat(i,j);
            }
            // memcpy(m_buffer+target_row+target_col, mat.m_buffer+source_row, sizeof(double) * mat.m_ncol);
        }
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    friend Matrix multiply_mkl(Matrix &mat1, Matrix &mat2);

private:
    size_t m_nrow;
    size_t m_ncol;
    // double * m_buffer;
    std::vector<double, MyAllocator<double>> m_buffer;

};

Matrix multiply_naive_bk(const Block &mat1, const Block &mat2) {
    size_t row=mat1.nrow();
    size_t col=mat2.ncol();
    size_t content=mat1.ncol();
    Matrix tmp(row, col);
    for (int i=0; i<row; i++) {
        for (int j=0; j<col; j++) {
            double sum=0.0;
            for (int k=0; k<content; k++) {
                sum+=mat1(i,k)*mat2(k,j);
            }
            tmp(i,j)=sum;
        }
    }
    return tmp;
}

Matrix multiply_naive(const Matrix &mat1, const Matrix &mat2) {
    size_t row=mat1.nrow();
    size_t col=mat2.ncol();
    size_t content=mat1.ncol();
    Matrix tmp(row, col, false);
    for (int i=0; i<row; i++) {
        for (int j=0; j<col; j++) {
            double sum=0.0;
            for (int k=0; k<content; k++) {
                sum+=mat1(i,k)*mat2(k,j);
            }
            tmp(i,j)=sum;
        }
    }
    return tmp;
}

Matrix multiply_tile(Matrix &mat1, Matrix &mat2, size_t block_size) {
    size_t row=mat1.nrow();
    size_t col=mat2.ncol();
    size_t content=mat1.ncol();
    Matrix result(row, col);
    int max_bk_row = row % block_size == 0 ? row/block_size : row/block_size+1;
    int max_bk_col = col % block_size == 0 ? col/block_size : col/block_size+1;
    int max_bk_content = content % block_size == 0 ? content/block_size : content/block_size+1;

    for (int i=0; i<max_bk_row; i++) {
        for (int j=0; j<max_bk_col; j++) {
            Matrix tmpmat(1,1);
            for (int k=0; k<max_bk_content; k++) {
                if (k==0) 
                    tmpmat = multiply_naive_bk(mat1.get_block(block_size, i, k, false), mat2.get_block(block_size, k, j, true));
                else
                    tmpmat +=  multiply_naive_bk(mat1.get_block(block_size, i, k, false), mat2.get_block(block_size, k, j, true));
            }
            result.set_block(block_size, i, j, tmpmat);
        }
    }
    return result;
}


Matrix multiply_mkl(Matrix &mat1, Matrix &mat2) {
    mkl_set_num_threads(1);

    Matrix ret(mat1.nrow(), mat2.ncol(), false);

    cblas_dgemm(
        CblasRowMajor /* const CBLAS_LAYOUT Layout */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transa */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transb */
      , mat1.nrow() /* const MKL_INT m */
      , mat2.ncol() /* const MKL_INT n */
      , mat1.ncol() /* const MKL_INT k */
      , 1.0 /* const double alpha */
      , mat1.m_buffer.data() /* const double *a */
      , mat1.ncol() /* const MKL_INT lda */
      , mat2.m_buffer.data() /* const double *b */
      , mat2.ncol() /* const MKL_INT ldb */
      , 0.0 /* const double beta */
      , ret.m_buffer.data() /* double * c */
      , ret.ncol() /* const MKL_INT ldc */
    );

    return ret;
}


PYBIND11_MODULE(_matrix, m) {
    m.doc() = "nsd21au hw3 pybind implementation"; // optional module docstring
    py::class_<Matrix>(m, "Matrix")
        .def(pybind11::init<int,int>())
        .def(pybind11::init<int,int,bool>())
        .def("__setitem__", [](Matrix &mat, std::pair<size_t, size_t> idx, double val) { return mat(idx.first, idx.second) = val; })
        .def("__getitem__", [](const Matrix &mat, std::pair<size_t, size_t> idx) { return mat(idx.first, idx.second); })
        // .def("__iadd__", [](Matrix &mat, const Matrix &mat2) { mat+=mat2; })
        // .def("__add__", [](const Matrix &mat1, const Matrix &mat2) { return mat1+mat2; })
        // .def("__isub__", [](Matrix &mat, const Matrix &mat2) { mat-=mat2; })
        // .def("__sub__", [](const Matrix &mat1, const Matrix &mat2) { return mat1-mat2; })
        .def("__eq__", [](const Matrix &mat1, const Matrix &mat2) { return mat1 == mat2; })
        // .def("getblock", &Matrix::get_block)
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol);

    // py::class_<Block>(m, "Block")
    //     .def(pybind11::init<int, int, bool>())
    //     .def("__getitem__", [](const Block &mat, std::pair<size_t, size_t> idx) { return mat(idx.first, idx.second); });
    
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);
    m.def("bytes", [](){ return GlobalAllocator.counter.bytes(); });
    m.def("allocated", [](){ return GlobalAllocator.counter.allocated(); });
    m.def("deallocated", [](){ return GlobalAllocator.counter.deallocated(); });
    m.def("meminfo",[](){
        std::cout << "\n*********************************************************************************" << std::endl;
        std::cout << "bytes:       " << GlobalAllocator.counter.bytes() << std::endl;
        std::cout << "allocated:   " << GlobalAllocator.counter.allocated() << std::endl;
        std::cout << "deallocated: " << GlobalAllocator.counter.deallocated() << std::endl;
        std::cout << "*********************************************************************************" << std::endl;
    });

}