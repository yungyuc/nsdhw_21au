#!/usr/bin/env python3

import numpy as np
import time
import pytest
import _matrix



#=======================================================================
class TestMatrix:

    def calculate(self, p, q, r, tsize):
        _m1 = np.random.rand(p, q)
        _m2 = np.random.rand(q, r)
        np_m = np.matmul(_m1, _m2)

        m1 = _matrix.Matrix(_m1)
        m2 = _matrix.Matrix(_m2)

        start = time.perf_counter()
        ret_naive = _matrix.multiply_naive(m1, m2)
        naive = time.perf_counter() - start

        start = time.perf_counter()
        ret_tile = _matrix.multiply_tile(m1, m2, tsize)
        tile = time.perf_counter() - start

        start = time.perf_counter()
        ret_mkl = _matrix.multiply_mkl(m1, m2)
        mkl = time.perf_counter() - start

        for i in range(p):
            for j in range(r):
                assert np_m[i, j] == approx(ret_naive[i, j])
                assert np_m[i, j] == approx(ret_tile[i, j])
                assert np_m[i, j] == approx(ret_mkl[i, j])

        return naive, tile, mkl

    def write_file(self, naive, tile, mkl):
        with open('performance.txt', 'w') as f:
            f.write('naive time: '+ str(naive) + " seconds\n")
            f.write('tile: '+ str(tile) + " seconds\n")
            f.write('mkl: '+ str(mkl) + " seconds\n")
            f.write('tile speeds up compared to naive: '+ str(naive / tile) + " times\n")
            f.write('MKL speeds up compared to naive: '+ str(naive / mkl) + " times\n")

    def test_1000(self):
        naive_min, tile_min, mkl_min = self.calculate(1000, 1000, 1000, 2)
        for i in range(2,6):
            naive, tile, mkl = self.calculate(1000, 1000, 1000, 2**i)
            naive_min = min(naive, naive_min)
            tile_min = min(tile, tile_min)
            mkl_min = min(mkl, mkl_min)
        self.write_file(naive_min, tile_min, mkl_min)
#======================================================================

