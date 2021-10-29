import _matrix
import pytest
import math
import os
import timeit

def test_zero_length():
    size = 5
    mat1 = _matrix.Matrix(size,size)
    mat2 = _matrix.Matrix(size,size)
    mat3 = _matrix.Matrix(size,size)

    for it in range(size):
        for jt in range(size):
            mat1[it, jt] = it * size + jt + 1
            mat2[it, jt] = it * size + jt + 1
            mat3[it, jt] = 0

    print(size)
    print(mat1)
    print(mat2)
    assert mat1.nrow == size
    assert mat1.ncol == size

def make_matrices(size):

    mat1 = _matrix.Matrix(size,size)
    mat2 = _matrix.Matrix(size,size)
    mat3 = _matrix.Matrix(size,size)

    for it in range(size):
        for jt in range(size):
            mat1[it, jt] = it * size + jt + 1
            mat2[it, jt] = it * size + jt + 1
            mat3[it, jt] = 0

    return mat1, mat2, mat3

def check_tile(mat1, mat2, tsize):

    if 0 == tsize:
        ret_tile = _matrix.multiply_naive(mat1, mat2)
        tile_str = "_matrix.multiply_naive(mat1, mat2)"
    else:
        ret_tile = _matrix.multiply_tile(mat1, mat2, tsize)
        tile_str = "_matrix.multiply_tile(mat1, mat2, tsize)"
    ret_mkl = _matrix.multiply_mkl(mat1, mat2)

    for i in range(ret_tile.nrow):
        for j in range(ret_tile.ncol):
            assert mat1[i,j] != ret_mkl[i,j]
            assert (ret_tile[i,j] == ret_mkl[i,j])

    ns = dict(_matrix=_matrix, mat1=mat1, mat2=mat2, tsize=tsize)
    t_tile = timeit.Timer(tile_str, globals=ns)
    t_mkl = timeit.Timer('_matrix.multiply_mkl(mat1, mat2)', globals=ns)

    time_tile = min(t_tile.repeat(10, 1))
    time_mkl = min(t_mkl.repeat(10, 1))
    ratio = time_tile/time_mkl

    return ratio, time_tile

def test_tile():

    show_ratio = bool(os.environ.get('SHOW_RATIO', False))

    mat1, mat2, *_ = make_matrices(500)

    ratio0, time0 = check_tile(mat1, mat2, 0)
    if show_ratio:
        print("naive ratio:", ratio0)

    ratio16, time16 = check_tile(mat1, mat2, 16)
    if show_ratio:
        print("tile 16 ratio:", ratio16)
        print("time16/time0:", time16/time0)
    assert (ratio16/ratio0 < 0.8)

    ratio17, time17 = check_tile(mat1, mat2, 17)
    if show_ratio:
        print("tile 17 ratio:", ratio17)
        print("time17/time0:", time17/time0)
    assert (ratio17/ratio0 < 0.8)

    ratio19, time19 = check_tile(mat1, mat2, 19)
    if show_ratio:
        print("tile 19 ratio:", ratio19)
        print("time19/time0:", time19/time0)
    assert (ratio19/ratio0 < 0.8)