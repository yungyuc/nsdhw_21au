#!/usr/bin/env python3
import _matrix
import numpy as np
import pytest
import math
import time


def is_mat_equal(mat1, mat2, eps=1e-6):
    if mat1.ncol != mat2.ncol or mat1.nrow != mat2.nrow:
        return False
    for i in range(mat1.nrow):
        for j in range(mat1.ncol):
            if not math.isclose(mat1[i,j], mat2[i,j], abs_tol=eps):
                return False
    return True


def np_random_mat(nrow, ncol):
    return np.random.rand(nrow, ncol) * 100.0

def np_to_matrix(npmat):
    mat = _matrix.Matrix(npmat.shape[0], npmat.shape[1])
    for i in range(npmat.shape[0]):
        for j in range(npmat.shape[1]):
            mat[i,j] = npmat[i][j]
    return mat

def test_naive():
    for _ in range(500):
        npm1 = np_random_mat(50, 50)
        npm2 = np_random_mat(50, 50)
        m1 = np_to_matrix(npm1)
        m2 = np_to_matrix(npm2)
        m3 = _matrix.multiply_naive(m1, m2)
        mref = np_to_matrix(np.matmul(npm1, npm2))
        assert is_mat_equal(m3, mref)

def test_tile():
    for _ in range(500):
        npm1 = np_random_mat(50, 50)
        npm2 = np_random_mat(50, 50)
        m1 = np_to_matrix(npm1)
        m2 = np_to_matrix(npm2)
        m3 = _matrix.multiply_tile(m1, m2, 16)
        mref = np_to_matrix(np.matmul(npm1, npm2))
        assert is_mat_equal(m3, mref)

def test_mkl():
    for _ in range(500):
        npm1 = np_random_mat(50, 50)
        npm2 = np_random_mat(50, 50)
        m1 = np_to_matrix(npm1)
        m2 = np_to_matrix(npm2)
        m3 = _matrix.multiply_mkl(m1, m2)
        mref = np_to_matrix(np.matmul(npm1, npm2))
        assert is_mat_equal(m3, mref)

def test_illegal():
    m1 = _matrix.Matrix(100, 30)
    m2 = _matrix.Matrix(100, 30)
    with pytest.raises(ValueError):
        _matrix.multiply_naive(m1, m2)
    with pytest.raises(ValueError):
        _matrix.multiply_tile(m1, m2, 16)
    with pytest.raises(ValueError):
        _matrix.multiply_mkl(m1, m2)

def test_speed():
    npm1 = np_random_mat(1000, 1000)
    npm2 = np_random_mat(1000, 1000)
    m1 = np_to_matrix(npm1)
    m2 = np_to_matrix(npm2)

    tstart = time.process_time()
    m_naive = _matrix.multiply_naive(m1, m2)
    tnaive = time.process_time() - tstart
    
    tstart = time.process_time()
    m_tile16 = _matrix.multiply_tile(m1, m2, 16)
    ttile16 = time.process_time() - tstart
    
    tstart = time.process_time()
    m_mkl = _matrix.multiply_mkl(m1, m2)
    tmkl = time.process_time() - tstart
    
    m_ref = np_to_matrix(np.matmul(npm1, npm2))

    
    with open('performance.txt', 'w') as f:
        f.write('multiply_naive: %f sec\n' % (tnaive))
        # f.write('multiply_tile 32: %f sec\n' % (ttile32))
        f.write('multiply_tile:  %f sec\n' % (ttile16))
        # f.write('multiply_tile 8: %f sec\n' % (ttile8))
        f.write('multiply_mkl:   %f sec\n' % (tmkl))
        # f.write('tiling32 is %f times faster than naive\n' % (tnaive/ttile32))
        f.write('tiling is %f times faster than naive\n' % (tnaive/ttile16))
        # f.write('tiling8  is %f times faster than naive\n' % (tnaive/ttile8))
        f.write('mkl    is %f times faster than naive\n' % (tnaive/tmkl))

    
    assert is_mat_equal(m_ref, m_naive)
    assert is_mat_equal(m_ref, m_tile16)
    assert is_mat_equal(m_ref, m_mkl)
    assert ttile16 < 0.8 * tnaive
