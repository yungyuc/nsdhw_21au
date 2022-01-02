import _matrix
import timeit
import os
import numpy as np


class TestClass:

    def make_matrices(self, size):
        mat1 = _matrix.Matrix(size,size)
        mat2 = _matrix.Matrix(size,size)
        mat3 = _matrix.Matrix(size,size)

        for it in range(size):
            for jt in range(size):
                mat1[it, jt] = it * size + jt + 1
                mat2[it, jt] = it * size + jt + 1
                mat3[it, jt] = 0

        return mat1, mat2, mat3

    def test_basic(self):
        size = 100
        mat1, mat2, mat3, *_ = self.make_matrices(size)

        assert(size == mat1.nrow)
        assert(size == mat1.ncol)
        assert(size == mat2.nrow)
        assert(size == mat2.ncol)
        assert(size == mat3.nrow)
        assert(size == mat3.ncol)
        assert(2    == mat1[0,1])
        assert(size+2 == mat1[1,1])
        assert(size*2 == mat1[1,size-1])
        assert(size*size ==  mat1[size-1,size-1])

        for i in range(mat1.nrow):
            for j in range(mat1.ncol):
                assert(0 != mat1[i, j])
                assert(mat1[i, j] ==  mat2[i, j])
                assert(0 == mat3[i, j])

        assert(mat1 == mat2)
        assert(mat1 is not mat2)

    def test_ndarray(self):

        size = 100
        mat = _matrix.Matrix(size,size)
        for it in range(size):
            for jt in range(size):
                mat[it, jt] = it * size + jt + 1

        for i in range(size):
            for j in range(size):
                assert(0 != mat[i, j])

        assert(True == isinstance(mat.array, np.ndarray))
        assert((size, size) == mat.array.shape)
        assert(np.dtype('float64') == mat.array.dtype)

        mat.array.fill(0)

        for i in range(size):
            for j in range(size):
                assert(0 == mat[i, j])


    def test_match(self):
        size = 100
        mat1, mat2, *_ = self.make_matrices(size)

        ret_naive = _matrix.multiply_naive(mat1, mat2)
        ret_mkl = _matrix.multiply_mkl(mat1, mat2)

        assert(size == ret_naive.nrow)
        assert(size == ret_naive.ncol)
        assert(size == ret_mkl.nrow)
        assert(size == ret_mkl.ncol)

        for i in range(ret_naive.nrow):
            for j in range(ret_naive.ncol):
                assert(mat1[i,j] != ret_mkl[i,j])
                assert(ret_naive[i,j] == ret_mkl[i,j])

    def test_zero(self):
        size = 100
        mat1, mat2, mat3, *_ = self.make_matrices(size)

        ret_naive = _matrix.multiply_naive(mat1, mat3)
        ret_mkl = _matrix.multiply_mkl(mat1, mat3)

        assert(size == ret_naive.nrow)
        assert(size == ret_naive.ncol)
        assert(size == ret_mkl.nrow)
        assert(size == ret_mkl.ncol)

        for i in range(ret_naive.nrow):
            for j in range(ret_naive.ncol):
                assert(0 == ret_naive[i,j])
                assert(0 == ret_mkl[i,j])
