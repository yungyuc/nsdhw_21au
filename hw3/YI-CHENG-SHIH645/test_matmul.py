from matrix import *
import time


def test_correctness():
    mat1 = Matrix(1024, 1024)
    mat2 = Matrix(1024, 1024)
    initialize(mat1)
    mat2 = mat1
    naive_ret = multiply_naive(mat1, mat2)
    tile_ret = multiply_tile(mat1, mat2, 64)
    mkl_ret = multiply_mkl(mat1, mat2)
    assert naive_ret == tile_ret
    assert tile_ret == mkl_ret


def test_performance():
    mat1 = Matrix(1024, 1024)
    mat2 = Matrix(1024, 1024)
    initialize(mat1)
    mat2 = mat1

    start = time.time()
    _ = multiply_naive(mat1, mat2)
    naive_time = time.time()-start

    start = time.time()
    _ = multiply_tile(mat1, mat2, 64)
    tile_time = time.time()-start

    start = time.time()
    _ = multiply_mkl(mat1, mat2)
    mkl_time = time.time()-start

    lines = [f'naive_time: {naive_time:.6f}\n',
             f'tile_time: {tile_time:.6f}\n',
             f'mkl_time: {mkl_time:.6f}\n']
    with open('performance.txt', 'w') as f:
        f.writelines(lines)
