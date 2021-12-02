#include <mkl.h>

#include<vector>
#include<stdexcept>

class Matrix {
public:
    Matrix(size_t nrow, size_t ncol) : m_nrow(nrow), m_ncol(ncol) {
        reset_buffer(nrow, ncol);
    }

    Matrix& operator=(Matrix const& other) {
        if (this == &other) { return *this; }
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) {
            reset_buffer(other.m_nrow, other.m_ncol);
        }
        for (size_t i = 0; i < m_nrow; ++i) {
            for (size_t j = 0; j < m_ncol; ++j) {
                (*this)(i,j) = other(i,j);
            }
        }

        return *this;
    }

    double operator()(size_t row, size_t col) const { return m_buffer[index(row, col)]; }
    double& operator() (size_t row, size_t col) { return m_buffer[index(row, col)]; }
    
    size_t index(size_t row, size_t col) const
    {
        return row * m_ncol + col;
    }

    size_t size() const {return m_nrow * m_ncol; }
    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double* m_buffer = nullptr;

    void reset_buffer(size_t nrow, size_t ncol)
    {
        if (m_buffer) { delete[] m_buffer; }
        const size_t nelement = nrow * ncol;
        if (nelement) { m_buffer = new double[nelement]; }
        else { m_buffer = nullptr; }
        m_nrow = nrow;
        m_ncol = ncol;
    }

};

bool operator==(Matrix const& m1, Matrix const& m2) {
    if ((m1.ncol() != m2.ncol()) && (m1.nrow() != m2.ncol()))
    {
        return false;
    }

    for (size_t i = 0; i < m1.nrow(); ++i)
    {
        for (size_t j = 0; j < m1.ncol(); ++j)
        {
            if (m1(i, j) != m2(i, j))
            {
                return false;
            }
        }
    }

    return true;
}

//naive matrix multiplication
Matrix multiply_naive(const Matrix &A, const Matrix &B) {
    if (A.ncol() != B.nrow()) {
        throw std::out_of_range("Matrix dimensions do not match."
            "Dimensions must be NxM * MxL.");
    }

    Matrix C(A.nrow(), B.ncol());

    for (size_t i = 0; i < A.nrow(); ++i) {
        for (size_t j = 0; j < B.ncol(); j++) {
            double res = 0;
            for (size_t k = 0; k < A.ncol(); ++k) {
                res += A(i, k) * B(k, j);
            }

            C(i, j) = res;
        }
    }

    return C;
}



//tile multiplication
Matrix multiply_tile(const Matrix &A, const Matrix &B, size_t Block) {
    if (A.ncol() != B.nrow()) {
        throw std::out_of_range("Matrix dimensions do not match."
            "Dimensions must be NxM * MxL.");
    }

    Matrix C(A.nrow(), B.ncol());

    for (size_t x = 0; x < C.nrow(); ++x) {
        for (size_t y = 0; y < C.ncol(); ++y) {
            C(x, y) = 0;
        }
    }

    size_t N = A.nrow();
    size_t s = Block;

    for (size_t i = 0; i < N; i += s) {
        for (size_t j = 0; j < N; j += s) {
            for (size_t k = 0; k < N; k += s) {
                for (size_t k1 = k; k1 < std::min(N, k + s); k1++) {
                    for (size_t i1 = i; i1 < std::min(N, i + s); i1++) {
                        for (size_t j1 = j; j1 < std::min(N, j + s); j1++) {
                            C(i1, j1) += A(i1, k1) * B(k1, j1);
                        }
                    }
                }
            }
        }
    }

    return C;

}

Matrix multiply_mkl(Matrix &mat1, Matrix &mat2){
	mkl_set_num_threads(1);

    Matrix ret(mat1.nrow(), mat2.ncol());

    cblas_dgemm(
        CblasRowMajor /* const CBLAS_LAYOUT Layout */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transa */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transb */
      , mat1.nrow() /* const MKL_INT m */
      , mat2.ncol() /* const MKL_INT n */
      , mat1.ncol() /* const MKL_INT k */
      , 1.0 /* const double alpha */
      , mat1.m_buffer /* const double *a */
      , mat1.ncol() /* const MKL_INT lda */
      , mat2.m_buffer /* const double *b */
      , mat2.ncol() /* const MKL_INT ldb */
      , 0.0 /* const double beta */
      , ret.m_buffer /* double * c */
      , ret.ncol() /* const MKL_INT ldc */
    );

    return ret;
}
