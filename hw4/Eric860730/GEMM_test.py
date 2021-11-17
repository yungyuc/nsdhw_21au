from _matrix import Matrix
import _matrix
import pytest
import timeit
import numpy as np
import os

def compareMat(mat1, mat2, m, n):
    for i in range(m):
        for j in range(n):
            if mat1[i,j]!= mat2[i,j]:
                print(f'm1[i,j]:{mat1[i,j]}')
                print(f'm2[i,j]:{mat2[i,j]}')
                return False
            return True

class Test_GEMM:

    def make_matrices(self, size):

        mat1 = Matrix(size,size)
        mat2 = Matrix(size,size)
        mat3 = Matrix(size,size)

        for it in range(size):
            for jt in range(size):
                mat1[it, jt] = it * size + jt + 1
                mat2[it, jt] = it * size + jt + 1
                mat3[it, jt] = 0

        return mat1, mat2, mat3

    def test_basic(self):

        assert 0 == _matrix.bytes()
        print(_matrix.bytes())
        size = 100
        mat1, mat2, mat3, *_ = self.make_matrices(size)
        print(_matrix.bytes())

        assert size == mat1.nrow
        assert size == mat1.ncol
        assert size == mat2.nrow
        assert size == mat2.ncol
        assert size == mat3.nrow
        assert size == mat3.ncol

        assert 2 == mat1[0,1]
        assert size+2 == mat1[1,1]
        assert size*2 == mat1[1,size-1]
        assert size*size == mat1[size-1,size-1]

        for i in range(mat1.nrow):
            for j in range(mat1.ncol):
                assert 0 != mat1[i,j]
                assert mat1[i,j] == mat2[i,j]
                assert 0 == mat3[i,j]

        assert mat1 == mat2
        assert mat1 is not mat2

    def test_match_naive_mkl(self):

        size = 100
        mat1, mat2, *_ = self.make_matrices(size)

        ret_naive = _matrix.multiply_naive(mat1, mat2)
        ret_mkl = _matrix.multiply_mkl(mat1, mat2)

        assert size == ret_naive.nrow
        assert size == ret_naive.ncol
        assert size == ret_mkl.nrow
        assert size == ret_mkl.ncol

        for i in range(ret_naive.nrow):
            for j in range(ret_naive.ncol):
                assert mat1[i,j] is not ret_mkl[i,j]
                assert ret_naive[i,j] == ret_mkl[i,j]

    def test_zero(self):

        size = 100
        mat1, mat2, mat3, *_ = self.make_matrices(size)

        ret_naive = _matrix.multiply_naive(mat1, mat3)
        ret_mkl = _matrix.multiply_mkl(mat1, mat3)

        assert size == ret_naive.nrow
        assert size == ret_naive.ncol
        assert size == ret_mkl.nrow
        assert size == ret_mkl.ncol

        for i in range(ret_naive.nrow):
            for j in range(ret_naive.ncol):
                assert 0 == ret_naive[i,j]
                assert 0 == ret_mkl[i,j]

    def check_tile(self, mat1, mat2, tsize):

        if 0 == tsize:
            ret_tile = _matrix.multiply_naive(mat1, mat2)
            tile_str = "_matrix.multiply_naive(mat1, mat2)"
        else:
            ret_tile = _matrix.multiply_tile(mat1, mat2, tsize)
            tile_str = "_matrix.multiply_tile(mat1, mat2, tsize)"
        ret_mkl = _matrix.multiply_mkl(mat1, mat2)

        for i in range(ret_tile.nrow):
            for j in range(ret_tile.ncol):
                assert mat1[i,j] is not ret_mkl[i,j]
                assert ret_tile[i,j] == ret_mkl[i,j]

        ns = dict(_matrix=_matrix, mat1=mat1, mat2=mat2, tsize=tsize)
        t_tile = timeit.Timer(tile_str, globals=ns)
        t_mkl = timeit.Timer('_matrix.multiply_mkl(mat1, mat2)', globals=ns)

        time_tile = min(t_tile.repeat(10, 1))
        time_mkl = min(t_mkl.repeat(10, 1))
        ratio = time_tile/time_mkl

        return ratio, time_tile

    def test_tile(self):

        show_ratio = bool(os.environ.get('SHOW_RATIO', False))

        mat1, mat2, *_ = self.make_matrices(500)

        ratio0, time0 = self.check_tile(mat1, mat2, 0)
        if show_ratio:
            print("naive ratio:", ratio0)

        ratio16, time16 = self.check_tile(mat1, mat2, 16)
        if show_ratio:
            print("tile 16 ratio:", ratio16)
            print("time16/time0:", time16/time0)
        assert ratio16/ratio0 < 0.8

        ratio17, time17 = self.check_tile(mat1, mat2, 17)
        if show_ratio:
            print("tile 17 ratio:", ratio17)
            print("time17/time0:", time17/time0)
        assert ratio17/ratio0 < 0.8

        ratio19, time19 = self.check_tile(mat1, mat2, 19)
        if show_ratio:
            print("tile 19 ratio:", ratio19)
            print("time19/time0:", time19/time0)
        assert ratio19/ratio0 < 0.8

    def test_matrix_info(self):
        row = 10
        col = 13
        m1 = Matrix(row, col)

        assert m1.nrow == row
        assert m1.ncol == col

    def test_construct(self):
        m,n =20,10
        m1 = Matrix(m,n)
        m2 = Matrix(m,n)
        for i in range(m):
            for j in range(n):
                m1[i,j] = 8.0
                m2[i,j] = 8.0
        assert(compareMat(m1,m2,m,n))


    def test_matrix_multiply_small_size(self):
        m1 = Matrix(2,3)
        m2 = Matrix(3,2)
        answer = Matrix(2,2)
        for i in range(2):
            for j in range(3):
                m1[i, j] = 3
                m2[j, i] = 3
        for i in range(2):
            for j in range(2):
                answer[i, j] = 27
        result_naive = _matrix.multiply_naive(m1, m2)
        result_tile = _matrix.multiply_tile(m1, m2, 1)
        result_mkl = _matrix.multiply_mkl(m1, m2)

        assert (compareMat(result_naive, answer, 2, 2))
        assert (compareMat(result_tile , answer, 2, 2))
        assert (compareMat(result_mkl , answer, 2, 2))

    def test_matrix_multiply_big_size(self):
        row = 30
        col = 20
        m1 = Matrix(row,col)
        m2 = Matrix(col,row)
        for i in range(row):
            for j in range(col):
                m1[i, j] = 6
                m2[j, i] = 4
        result_naive = _matrix.multiply_naive(m1, m2)
        result_tile = _matrix.multiply_tile(m1, m2, 10)
        result_mkl = _matrix.multiply_mkl(m1, m2)

        assert (compareMat(result_naive, result_tile, row, row))
        assert (compareMat(result_naive, result_mkl, row, row))

