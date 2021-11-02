#!/usr/bin/env python3

import pytest
import _matrix as mtx
import numpy as np

def test_multiply_naive():
    m = np.random.randint(1, 20)
    n = np.random.randint(1, 20)
    k = np.random.randint(1, 20)

    mat1 = mtx.Matrix(m, k)
    for i in range(m):
        for j in range(k):
            mat1[i, j] = 100 * (np.random.random() * 2 - 1)

    mat2 = mtx.Matrix(k, n)
    for i in range(k):
        for j in range(n):
            mat2[i, j] = 100 * (np.random.random() * 2 - 1)

    mat3_naive = mtx.multiply_naive(mat1, mat2)
    mat3_mkl = mtx.multiply_mkl(mat1, mat2)

    assert mat3_naive.nrow == mat3_mkl.nrow
    assert mat3_naive.ncol == mat3_mkl.ncol
    for i in range(m):
        for j in range(n):
            assert abs(mat3_naive[i, j] - mat3_mkl[i, j]) < 1e-4

def test_multiply_tile():
    m = np.random.randint(16, 32+1)
    n = np.random.randint(16, 32+1)
    k = np.random.randint(16, 32+1)

    mat1 = mtx.Matrix(m, k)
    for i in range(m):
        for j in range(k):
            mat1[i, j] = 100 * (np.random.random() * 2 - 1)

    mat2 = mtx.Matrix(k, n)
    for i in range(k):
        for j in range(n):
            mat2[i, j] = 100 * (np.random.random() * 2 - 1)

    mat3_mkl = mtx.multiply_mkl(mat1, mat2)
    for t_size in [2, 4, 8, 16]:
        mat3_tile = mtx.multiply_tile(mat1, mat2, t_size)
        assert mat3_tile.nrow == mat3_mkl.nrow
        assert mat3_tile.ncol == mat3_mkl.ncol
        for i in range(m):
            for j in range(n):
                assert abs(mat3_tile[i, j] - mat3_mkl[i, j]) < 1e-4

