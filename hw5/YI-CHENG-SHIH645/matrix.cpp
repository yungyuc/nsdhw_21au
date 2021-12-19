#include <iostream>
#include <chrono>
#include <vector>
#include <mkl.h>
#include <pybind11/pybind11.h>
#include "matrix.hpp"
namespace py = pybind11;

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
                mat1.m_buffer, (int)mat1.ncol(),
                mat2.m_buffer, (int)mat2.ncol(), 0.0,
                ret.m_buffer, (int)ret.ncol());
    return ret;
}
