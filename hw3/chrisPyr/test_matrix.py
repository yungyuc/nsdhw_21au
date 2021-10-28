import sys
import os
import timeit
import unittest
import time

# The python module that wraps the matrix code.
import _matrix


class GradingTest(unittest.TestCase):

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

        size = 1000
        mat1, mat2, mat3, *_ = self.make_matrices(size)

        self.assertEqual(size, mat1.nrow)
        self.assertEqual(size, mat1.ncol)
        self.assertEqual(size, mat2.nrow)
        self.assertEqual(size, mat2.ncol)
        self.assertEqual(size, mat3.nrow)
        self.assertEqual(size, mat3.ncol)

        self.assertEqual(2, mat1[0,1])
        self.assertEqual(size+2, mat1[1,1])
        self.assertEqual(size*2, mat1[1,size-1])
        self.assertEqual(size*size, mat1[size-1,size-1])

        for i in range(mat1.nrow):
            for j in range(mat1.ncol):
                self.assertNotEqual(0, mat1[i,j])
                self.assertEqual(mat1[i,j], mat2[i,j])
                self.assertEqual(0, mat3[i,j])

        self.assertEqual(mat1, mat2)
        self.assertTrue(mat1 is not mat2)

    def test_match_naive_mkl(self):

        size = 1000
        mat1, mat2, *_ = self.make_matrices(size)

        ret_naive = _matrix.multiply_naive(mat1, mat2)
        ret_mkl = _matrix.multiply_mkl(mat1, mat2)

        self.assertEqual(size, ret_naive.nrow)
        self.assertEqual(size, ret_naive.ncol)
        self.assertEqual(size, ret_mkl.nrow)
        self.assertEqual(size, ret_mkl.ncol)

        for i in range(ret_naive.nrow):
            for j in range(ret_naive.ncol):
                self.assertNotEqual(mat1[i,j], ret_mkl[i,j])
                self.assertEqual(ret_naive[i,j], ret_mkl[i,j])

    def test_match_naive_tile(self):
        size = 1000
        mat1, mat2, *_ = self.make_matrices(size)

        ret_naive = _matrix.multiply_naive(mat1, mat2)
        ret_tile = _matrix.multiply_tile(mat1, mat2, 64)

        self.assertEqual(size, ret_naive.nrow)
        self.assertEqual(size, ret_naive.ncol)
        self.assertEqual(size, ret_tile.nrow)
        self.assertEqual(size, ret_tile.ncol)

        for i in range(ret_naive.nrow):
            for j in range(ret_naive.ncol):
                self.assertNotEqual(mat1[i,j], ret_tile[i,j])
                self.assertEqual(ret_naive[i,j], ret_tile[i,j])

    def test_zero(self):

        size = 1000
        mat1, mat2, mat3, *_ = self.make_matrices(size)

        ret_naive = _matrix.multiply_naive(mat1, mat3)
        ret_mkl = _matrix.multiply_mkl(mat1, mat3) 
        self.assertEqual(size, ret_naive.nrow)
        self.assertEqual(size, ret_naive.ncol)
        self.assertEqual(size, ret_mkl.nrow)
        self.assertEqual(size, ret_mkl.ncol)

        for i in range(ret_naive.nrow):
            for j in range(ret_naive.ncol):
                self.assertEqual(0, ret_naive[i,j])
                self.assertEqual(0, ret_mkl[i,j])

    def test_tile(self):
        size = 1000
        mat1, mat2, mat3, *_ = self.make_matrices(size)

        with open("performance.txt","a") as per_writer:
            tile_size = [16, 32, 64]
            for i in tile_size:
                time_start = time.time()
                res_mat = _matrix.multiply_tile(mat1,mat2,i)
                time_end = time.time()
                per_writer.write("tile_size:{}, {} sec\n".format(i,(time_end - time_start)))



    def test_performance(self):
        size = 1000
        mat1, mat2, mat3, *_ = self.make_matrices(size)

        time_start = time.time()
        _matrix.multiply_naive(mat1, mat2)
        time_end = time.time()
        time_naive = time_end - time_start;

        time_start = time.time()
        _matrix.multiply_tile(mat1, mat2, 16)
        time_end = time.time()
        time_tile = time_end - time_start;

        time_start = time.time()
        _matrix.multiply_mkl(mat1, mat2)
        time_end = time.time()
        time_mkl = time_end - time_start;

        with open("performance.txt","a") as per_writer:
            per_writer.write("Naive time: {} sec\n".format(time_naive))
            per_writer.write("Tile time: {} sec\n".format(time_tile))
            per_writer.write("Tile speed up over Naive: {}x\n".format(time_naive/time_tile))
            per_writer.write("MKL time: {} sec\n".format(time_mkl))
            per_writer.write("MKL speed up over Naive: {}x\n".format(time_naive/time_mkl))
            per_writer.write("MKL speed up over Tile: {}x\n".format(time_tile/time_mkl))





