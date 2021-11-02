#!/usr/bin/env python3

import _matrix
import numpy as np
import pytest
import random
import time

def is_approx_equal(mat1, mat2, m, n):
    for i in range(m):
        for j in range(n):
            assert mat1[i, j] == pytest.approx(mat2[i, j])
    return True

def run_benchmark(p, q, r, tsize):
    mat1 = np.random.rand(p, q)
    mat2 = np.random.rand(q, r)
    gt = np.matmul(mat1, mat2)

    _mat1 = _matrix.Matrix(mat1)
    _mat2 = _matrix.Matrix(mat2)

    start = time.process_time()
    ret_naive = _matrix.multiply_naive(_mat1, _mat2)
    t_naive = time.process_time() - start

    start = time.process_time()
    ret_tile = _matrix.multiply_tile(_mat1, _mat2, tsize)
    t_tile = time.process_time() - start

    start = time.process_time()
    ret_mkl = _matrix.multiply_mkl(_mat1, _mat2)
    t_mkl = time.process_time() - start

    assert(is_approx_equal(gt, ret_naive, p, r))
    assert(is_approx_equal(gt, ret_tile, p, r))
    assert(is_approx_equal(gt, ret_mkl, p, r))

    with open('performance.txt', 'w') as f:
        f.write(f'Setup:\n p: {p} q: {q} r: {r} tsize: {tsize}\n')
        f.write(f'Naive: {t_naive}s\n')
        f.write(f'Tile: {t_tile}s\n')
        f.write(f'MKL: {t_mkl}s\n')
        f.write(f'Ratio of Naive/Tile: {(t_naive / t_tile) * 100}%\n')
        f.write(f'Ratio of Naive/MKL: {(t_naive / t_mkl) * 100}%\n')

def test_1000():
    run_benchmark(1000, 1000, 1000, 4)

def test_random():
    p = random.randint(1, 1000)
    q = random.randint(1, 1000)
    r = random.randint(1, 1000)
    tsize = random.randint(1, 1000)
    run_benchmark(p, q, r, tsize)