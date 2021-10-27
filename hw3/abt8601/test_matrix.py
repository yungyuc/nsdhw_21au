import random

from _matrix import Matrix


def test_matrix_get_set() -> None:
    N_TESTS = 10

    for _ in range(N_TESTS):
        nrow = random.randint(1, 100)
        ncol = random.randint(1, 100)
        template = [[random.random() for _ in range(ncol)]
                    for _ in range(nrow)]

        mat = Matrix(nrow, ncol)

        for i in range(nrow):
            for j in range(ncol):
                mat[i, j] = template[i][j]

        for i in range(nrow):
            for j in range(ncol):
                assert mat[i, j] == template[i][j]
