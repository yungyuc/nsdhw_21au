#!/usr/bin/env python3

import _matrix
from pytest import approx
import numpy as np
import random
import time

class TestMatrix:

    def multiply_calculate(self, row, col1, col2, tsize):
        _m1 = np.random.rand(row, col1)
        _m2 = np.random.rand(col1, col2)
        np_m = np.matmul(_m1, _m2)

        m1 = _matrix.Matrix(_m1)
        m2 = _matrix.Matrix(_m2)

        #naive
        time_start = time.time()
        ret_naive = _matrix.multiply_naive(m1, m2)
        time_end = time.time()
        naive = time_end - time_start

        #tile
        time_start = time.time()
        ret_tile = _matrix.multiply_tile(m1, m2, tsize)
        time_end = time.time()
        tile = time_end - time_start

        #mkl
        time_start = time.time()
        ret_mkl = _matrix.multiply_mkl(m1, m2)
        time_end = time.time()
        mkl = time_end - time_start

        for i in range(row):
            for j in range(col2):
                assert np_m[i, j] == approx(ret_naive[i, j])
                assert np_m[i, j] == approx(ret_tile[i, j])
                assert np_m[i, j] == approx(ret_mkl[i, j])

        return naive, tile, mkl

    def write_file(self, naive_t, tile_t, mkl_t):
        with open('performance.txt', 'w') as f:
            f.write('mutiply_naive time: '+ str(naive_t) + " second\n")
            f.write('mutiply_tile: '+ str(tile_t) + " second\n")
            f.write('mutiply_mkl: '+ str(mkl_t) + " second\n")
            f.write('tile speeds up rate ( naive time / tile time) : '+ str(naive_t / tile_t) + "\n")
            f.write('MKL speeds up rate (naive time / mkl time) : '+ str(naive_t / mkl_t) + "\n")




    def test_1000(self):
        naive_min, tile_min, mkl_min = self.multiply_calculate(1000, 1000, 1000, 2)
        for i in range(2,6):
            naive, tile, mkl = self.multiply_calculate(1000, 1000, 1000, 2**i)
            naive_min = min(naive, naive_min)
            tile_min = min(tile, tile_min)
            mkl_min = min(mkl, mkl_min)
        self.write_file(naive_min, tile_min, mkl_min)