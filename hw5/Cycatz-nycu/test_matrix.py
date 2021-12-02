#!/usr/bin/env python3
from _matrix import Matrix, multiply_naive, multiply_tile, multiply_mkl

import math
import random
import time
import pytest

def init_matrix(M, N, L):
    m1 = Matrix(M, N)
    m2 = Matrix(N, L)

    for i in range(M):
        for j in range(N):
            m1[i, j] = random.random()

    for i in range(N):
        for j in range(L):
            m2[i, j] = random.random()

    return (m1, m2)

def is_equal(m1, m2):
    if (m1.ncol != m2.ncol) or (m1.nrow != m2.nrow):
        return False
    for i in range(m1.nrow):
        for j in range(m1.ncol):
            if not math.isclose(m1[i, j], m2[i, j], abs_tol=1e-6):
                return False
    return True


def test_can_not_multiply():
    m1 = Matrix(10, 20)
    m2 = Matrix(30, 100)
    with pytest.raises(ValueError):
        multiply_naive(m1, m2)
    with pytest.raises(ValueError):
        multiply_tile(m1, m2, 16)
    with pytest.raises(ValueError):
        multiply_mkl(m1, m2)

def test_speed():
    M = 1000
    N = 1000
    L = 1000

    m1, m2 = init_matrix(M, N, L)

    tstart = time.process_time()
    m_naive = multiply_naive(m1, m2)
    tnaive = time.process_time() - tstart

    tstart = time.process_time()
    m_tile8 = multiply_tile(m1, m2, 8)
    ttile8 = time.process_time() - tstart
    
    tstart = time.process_time()
    m_tile16 = multiply_tile(m1, m2, 16)
    ttile16 = time.process_time() - tstart


    tstart = time.process_time()
    m_tile32 = multiply_tile(m1, m2, 32)
    ttile32 = time.process_time() - tstart


    tstart = time.process_time()
    m_mkl = multiply_mkl(m1, m2)
    tmkl = time.process_time() - tstart

    with open('performance.txt', 'w') as f:
        f.write('multiply_naive: %f sec\n' % (tnaive))
        f.write('multiply_tile (8):  %f sec\n' % (ttile8))
        f.write('multiply_tile (16):  %f sec\n' % (ttile16))
        f.write('multiply_tile (32):  %f sec\n' % (ttile32))
        f.write('multiply_mkl:   %f sec\n' % (tmkl))
        f.write('tiling (8) with is %f times faster than naive\n' % (tnaive/ttile8))
        f.write('tiling (16) with is %f times faster than naive\n' % (tnaive/ttile16))
        f.write('tiling (32) with is %f times faster than naive\n' % (tnaive/ttile32))
        f.write('mkl is %f times faster than naive\n' % (tnaive/tmkl))

    assert is_equal(m_naive, m_tile8)
    assert is_equal(m_naive, m_tile16)
    assert is_equal(m_naive, m_tile32)
    assert is_equal(m_naive, m_mkl)



