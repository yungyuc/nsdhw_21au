#!/usr/bin/env python3

import _matrix
import numpy as np
import pytest
import random
import time

def is_approx_equal(m1, m2, m, n):
    for i in range(m):
        for j in range(n):
            assert m1[i, j] == pytest.approx(m2[i, j])
    return True

def run_benchmark(p, q, r, tsize):
    m1 = np.random.rand(p, q)
    m2 = np.random.rand(q, r)
    m3 = np.matmul(m1, m2)

    _m1 = _matrix.Matrix(m1)
    _m2 = _matrix.Matrix(m2)

    start = time.process_time()
    ret_naive = _matrix.multiply_naive(_m1, _m2)
    t_naive = time.process_time() - start

    start = time.process_time()
    ret_tile = _matrix.multiply_tile(_m1, _m2, tsize)
    t_tile = time.process_time() - start

    start = time.process_time()
    ret_mkl = _matrix.multiply_mkl(_m1, _m2)
    t_mkl = time.process_time() - start

    assert m3.shape[0] == ret_naive.nrow and m3.shape[1] == ret_naive.ncol
    assert m3.shape[0] == ret_tile.nrow and m3.shape[1] == ret_tile.ncol
    assert m3.shape[0] == ret_mkl.nrow and m3.shape[1] == ret_mkl.ncol

    assert(is_approx_equal(m3, ret_naive, p, r))
    assert(is_approx_equal(m3, ret_tile, p, r))
    assert(is_approx_equal(m3, ret_mkl, p, r))

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