#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <math.h>
#include <mkl.h>

class Matrix
{

public:
    Matrix(size_t nrow, size_t ncol)
        : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
    }

    Matrix(size_t nrow, size_t ncol, std::vector<double> const &vec)
        : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
        (*this) = vec;
    }

    Matrix &operator=(std::vector<double> const &vec)
    {
        if (size() != vec.size())
        {
            throw std::out_of_range("number of elements mismatch");
        }

        size_t k = 0;
        for (size_t i = 0; i < m_nrow; ++i)
        {
            for (size_t j = 0; j < m_ncol; ++j)
            {
                (*this)(i, j) = vec[k];
                ++k;
            }
        }

        return *this;
    }

    Matrix(Matrix const &other)
        : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
        reset_buffer(other.m_nrow, other.m_ncol);
        for (size_t i = 0; i < m_nrow; ++i)
        {
            for (size_t j = 0; j < m_ncol; ++j)
            {
                (*this)(i, j) = other(i, j);
            }
        }
    }

    Matrix &operator=(Matrix const &other)
    {
        if (this == &other)
        {
            return *this;
        }
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol)
        {
            reset_buffer(other.m_nrow, other.m_ncol);
        }
        for (size_t i = 0; i < m_nrow; ++i)
        {
            for (size_t j = 0; j < m_ncol; ++j)
            {
                (*this)(i, j) = other(i, j);
            }
        }
        return *this;
    }

    Matrix(Matrix &&other)
        : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
    }

    Matrix &operator=(Matrix &&other)
    {
        if (this == &other)
        {
            return *this;
        }
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
        return *this;
    }

    ~Matrix()
    {
        reset_buffer(0, 0);
    }

    bool operator==(Matrix const &other) const
    {
        if ((m_nrow != other.nrow()) || (m_ncol != other.ncol()))
            return false;

        for (size_t i = 0; i < m_nrow; i++)
            for (size_t j = 0; j < m_ncol; j++)
                if (m_buffer[index(i, j)] != other(i, j))
                    return false;

        return true;
    }

    double operator()(size_t row, size_t col) const
    {
        return m_buffer[index(row, col)];
    }
    double &operator()(size_t row, size_t col)
    {
        return m_buffer[index(row, col)];
    }

    double get_data(std::pair<size_t, size_t> coord)
    {
        return m_buffer[index(coord.first, coord.second)];
    }
    void set_data(std::pair<size_t, size_t> coord, const double val)
    {
        m_buffer[index(coord.first, coord.second)] = val;
    }

    double* raw() const{
        return m_buffer;
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    size_t size() const { return m_nrow * m_ncol; }
    double buffer(size_t i) const { return m_buffer[i]; }
    std::vector<double> buffer_vector() const
    {
        return std::vector<double>(m_buffer, m_buffer + size());
    }

private:
    size_t index(size_t row, size_t col) const
    {
        return row + col * m_nrow;
    }

    void reset_buffer(size_t nrow, size_t ncol)
    {
        if (m_buffer)
        {
            delete[] m_buffer;
        }
        const size_t nelement = nrow * ncol;
        if (nelement)
        {
            m_buffer = new double[nelement]{0};
        }
        else
        {
            m_buffer = nullptr;
        }
        m_nrow = nrow;
        m_ncol = ncol;
    }

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double *m_buffer = nullptr;
};

/*
 * Naive matrix matrix multiplication.
 */
Matrix operator*(Matrix const &mat1, Matrix const &mat2)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    Matrix ret(mat1.nrow(), mat2.ncol());

    for (size_t i = 0; i < ret.nrow(); ++i)
    {
        for (size_t k = 0; k < ret.ncol(); ++k)
        {
            double v = 0;
            for (size_t j = 0; j < mat1.ncol(); ++j)
            {
                v += mat1(i, j) * mat2(j, k);
            }
            ret(i, k) = v;
        }
    }

    return ret;
}

std::ostream &operator<<(std::ostream &ostr, Matrix const &mat)
{
    for (size_t i = 0; i < mat.nrow(); ++i)
    {
        ostr << std::endl
             << " ";
        for (size_t j = 0; j < mat.ncol(); ++j)
        {
            ostr << " " << std::setw(2) << mat(i, j);
        }
    }

    return ostr;
}

/*
 * Naive matrix matrix multiplication.
 */
Matrix multiply_naive(Matrix const &mat1, Matrix const &mat2)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    Matrix ret(mat1.nrow(), mat2.ncol());

    for (size_t i = 0; i < ret.nrow(); ++i)
    {
        for (size_t k = 0; k < ret.ncol(); ++k)
        {
            double v = 0;
            for (size_t j = 0; j < mat1.ncol(); ++j)
            {
                v += mat1(i, j) * mat2(j, k);
            }
            ret(i, k) = v;
        }
    }

    return ret;
}

/*
 * Tile matrix matrix multiplication.
 */
Matrix multiple_tile(Matrix const &mat1, Matrix const &mat2, const size_t width)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    const size_t M = mat1.nrow(), N = mat1.ncol(), K = mat2.ncol();
    Matrix ret(M, K);

    for (size_t row = 0; row < M; row += width)
    {
        for (size_t col = 0; col < K; col += width)
        {
            for (size_t iter = 0; iter < N; iter += width)
            {
                for (size_t m = row; m < std::min(M, row + width); ++m)
                {
                    for (size_t k = col; k < std::min(K, col + width); ++k)
                    {
                        for (size_t n = iter; n < std::min(N, iter + width); ++n)
                        {
                            // ret(m, k) += mat1(m, n) * mat2(n, k);
                            ret.buffer(m*N + k) += mat1(m*N + n) * mat2(n*N + k);
                        }
                    }
                }
            }
        }
    }

    return ret;
}

/*
 * mkl matrix matrix multiplication.
 */
Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2)
{
    // return multiply_naive(mat1, mat2);
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    Matrix ret(mat1.nrow(), mat2.ncol());

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                mat1.nrow(), mat2.ncol(), mat1.ncol(), 1, mat1.raw(),
                mat1.ncol(), mat2.raw(), mat2.ncol(), 0, ret.raw(), ret.ncol());

    return ret;
}

PYBIND11_MODULE(_matrix, m)
{
    m.def("multiply_naive", &multiply_naive, "Native matrix matrix multiplication.");
    m.def("multiply_tile", &multiple_tile, "Tile matrix matrix multiplication.");
    m.def("multiply_mkl", &multiply_mkl, "mkl matrix matrix multiplication.");

    pybind11::class_<Matrix>(m, "Matrix")
        .def(pybind11::init<const size_t, const size_t>())
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def("__getitem__", &Matrix::get_data)
        .def("__setitem__", &Matrix::set_data)
        .def(pybind11::self == pybind11::self);
}
