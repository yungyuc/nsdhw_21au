import gc
import random

import _matrix


def get_expected_matrix_size(nrow, ncol):
    SIZEOF_ELEM = 8
    return nrow * ncol * SIZEOF_ELEM


def test_matrix_alloc():
    N_TESTS = 100

    gc.collect()

    for _ in range(N_TESTS):
        # Initially nothing is alive
        assert _matrix.bytes() == 0

        allocated_start = _matrix.allocated()
        deallocated_start = _matrix.deallocated()

        # Allocate a matrix
        nrow = random.randint(1, 100)
        ncol = random.randint(1, 100)
        mat = _matrix.Matrix(nrow, ncol)

        # Check that the allocation size is correct
        expected_size = get_expected_matrix_size(nrow, ncol)
        assert _matrix.bytes() == expected_size
        assert _matrix.allocated() == allocated_start + expected_size
        assert _matrix.deallocated() == deallocated_start

        # Deallocate the matrix
        del mat
        gc.collect()

        # Check that the allocation size is correct
        assert _matrix.bytes() == 0
        assert _matrix.allocated() == allocated_start + expected_size
        assert _matrix.deallocated() == deallocated_start + expected_size
