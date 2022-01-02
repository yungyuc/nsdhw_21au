import math
import random

import numpy as np
import pytest

from _matrix import Matrix, multiply_naive, multiply_tile, multiply_mkl


def test_matrix_get_set() -> None:
    N_TESTS = 5
    OOB_N_TESTS = 5

    for _ in range(N_TESTS):
        nrow = random.randint(1, 500)
        ncol = random.randint(1, 500)
        template = [[random.random() for _ in range(ncol)]
                    for _ in range(nrow)]

        # Matrix allocation.
        mat = Matrix(nrow, ncol)

        # Dimension check.
        assert mat.nrow == nrow
        assert mat.ncol == ncol

        # Set.
        for i in range(nrow):
            for j in range(ncol):
                mat[i, j] = template[i][j]

        # Get check.
        for i in range(nrow):
            for j in range(ncol):
                assert mat[i, j] == template[i][j]

        # Out-of-bounds get check.
        for _ in range(OOB_N_TESTS):
            with pytest.raises(IndexError):
                mat[random.randint(0, nrow-1), random.randint(ncol, 1000)]
            with pytest.raises(IndexError):
                mat[random.randint(nrow, 1000), random.randint(0, ncol-1)]
            with pytest.raises(IndexError):
                mat[random.randint(nrow, 1000), random.randint(ncol, 1000)]

        # Out-of-bounds set check.
        for _ in range(OOB_N_TESTS):
            with pytest.raises(IndexError):
                mat[random.randint(0, nrow-1),
                    random.randint(ncol, 1000)] = random.random()
            with pytest.raises(IndexError):
                mat[random.randint(nrow, 1000), random.randint(
                    0, ncol-1)] = random.random()
            with pytest.raises(IndexError):
                mat[random.randint(nrow, 1000), random.randint(
                    ncol, 1000)] = random.random()


def test_matrix_zs() -> None:
    N_TESTS = 5

    for _ in range(N_TESTS):
        nrow = random.randint(1, 500)
        ncol = random.randint(1, 500)

        # Matrix allocation.
        Matrix(0, ncol)
        Matrix(nrow, 0)


def test_multiply() -> None:
    N_TESTS = 1000

    for _ in range(N_TESTS):
        m = random.randint(1, 100)
        n = random.randint(1, 100)
        k = random.randint(1, 100)
        tile_size = random.randint(1, min(m, n, k))

        mat1 = Matrix(m, k)
        for i in range(m):
            for j in range(k):
                mat1[i, j] = random.random()

        mat2 = Matrix(k, n)
        for i in range(k):
            for j in range(n):
                mat2[i, j] = random.random()

        mat3_naive = multiply_naive(mat1, mat2)
        mat3_tile = multiply_tile(mat1, mat2, tile_size)
        mat3_mkl = multiply_mkl(mat1, mat2)

        assert mat3_naive.nrow == m
        assert mat3_naive.ncol == n
        assert mat3_tile.nrow == m
        assert mat3_tile.ncol == n
        assert mat3_mkl.nrow == m
        assert mat3_mkl.ncol == n

        assert mat3_naive == mat3_tile
        assert all(math.isclose(mat3_naive[i, j], mat3_mkl[i, j])
                   for i in range(m) for j in range(n))


def test_multiply_dim_mismatch() -> None:
    N_TESTS = 5

    for _ in range(N_TESTS):
        m = random.randint(1, 100)
        n = random.randint(1, 100)
        k = random.randint(1, 100)
        while (k2 := random.randint(1, 100)) == k:
            pass
        tile_size = random.randint(1, min(m, n, k, k2))

        mat1 = Matrix(m, k)
        mat2 = Matrix(k2, n)

        with pytest.raises(ValueError):
            multiply_naive(mat1, mat2)
        with pytest.raises(ValueError):
            multiply_tile(mat1, mat2, tile_size)
        with pytest.raises(ValueError):
            multiply_mkl(mat1, mat2)


def test_multiply_zs() -> None:
    N_TESTS = 5

    def test_case(m: int, n: int, k: int, tile_size: int) -> None:
        mat1 = Matrix(m, k)
        mat2 = Matrix(k, n)

        mat3_naive = multiply_naive(mat1, mat2)
        mat3_tile = multiply_tile(mat1, mat2, tile_size)
        mat3_mkl = multiply_mkl(mat1, mat2)

        assert mat3_naive.nrow == m
        assert mat3_naive.ncol == n
        assert mat3_tile.nrow == m
        assert mat3_tile.ncol == n
        assert mat3_mkl.nrow == m
        assert mat3_mkl.ncol == n

        assert mat3_naive == mat3_tile
        assert mat3_naive == mat3_mkl

    for _ in range(N_TESTS):
        m = random.randint(1, 100)
        n = random.randint(1, 100)
        k = random.randint(1, 100)
        tile_size = random.randint(1, min(m, n, k))

        test_case(0, n, k, tile_size)
        test_case(m, 0, k, tile_size)
        test_case(m, n, 0, tile_size)


def test_numpy() -> None:
    N_TESTS = 10

    for _ in range(N_TESTS):
        nrow = random.randint(1, 500)
        ncol = random.randint(1, 500)
        template = [[random.random() for _ in range(ncol)]
                    for _ in range(nrow)]

        mat = Matrix(nrow, ncol)

        # Check that mat.array is a NumPy array with the correct shape.
        assert(isinstance(mat.array, np.ndarray))
        assert(mat.array.shape == (nrow, ncol))

        # Set values via the NumPy array.
        mat.array[:] = template

        # Check that the values are correct.
        for i in range(nrow):
            for j in range(ncol):
                assert(mat[i, j] == template[i][j])

        # Test memory management.

        arr = mat.array
        del mat

        # Check that the NumPy array is still valid.
        assert(np.all(arr == template))
