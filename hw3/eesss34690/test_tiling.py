import numpy as np
import numpy.testing as npt
import _matrix as m
import timeit

size = 1000  ## for basic test

def copy_matrix(mat1, mat2, nrow, ncol):
    for i in range(nrow*ncol):
        mat1[i] = mat2[i]

def np_matrix(nrow, ncol):
    mat = np.arange(nrow*ncol, dtype=np.float) - int(nrow*ncol/2)
    return mat.reshape((nrow, ncol))

def test_setitem():
    mat = m.Matrix(2, 3)
    for i in range(2):
        for j in range(2):
            mat[i, j] = 2.3

def test_getitem():
    mat = m.Matrix(2, 3)
    for i in range(2):
        for j in range(3):
            tmp = mat[i, j]

def test_attributes():
    mat = m.Matrix(2, 3)
    npt.assert_equal(2, mat.nrow)
    npt.assert_equal(3, mat.ncol)
    flag = True
    try:
        mat.nrow == 2
    except AttributeError:
        pass
    except:
        flag = False
    try:
        mat.ncol == 3
    except AttributeError:
        pass
    except:
        flag = False

    return npt.assert_equal(flag, True)

def test_no_match_size():
    npt.assert_raises(IndexError, m.multiply_naive, m.Matrix(2, 3), m.Matrix(2,
    3))
    npt.assert_raises(IndexError, m.multiply_tile, m.Matrix(2, 3), m.Matrix(2,
    3), 2)

def test_naive():
    mat1 = m.Matrix(size, size)
    mat2 = m.Matrix(size, size)
    np_mat = np_matrix(size, size)

    copy_matrix(mat1, np_mat, size, size)
    copy_matrix(mat2, np_mat, size, size)

    mat_naive = m.multiply_naive(mat1, mat2)
    mat_mkl = m.multiply_mkl(mat1, mat2)

    for i in range(size):
        for j in range(size):
            npt.assert_equal(mat_naive[i, j], mat_mkl[i, j])


def test_tile():
    print("start tile")
    mat1 = m.Matrix(size, size)
    print("1")
    mat2 = m.Matrix(size, size)
    print("2")
    np_mat = np_matrix(size, size)

    copy_matrix(mat1, np_mat, size, size)
    copy_matrix(mat2, np_mat, size, size)

    mat_tile = m.multiply_tile(mat1, mat2, 8)
    mat_mkl = m.multiply_mkl(mat1, mat2)

    for i in range(size):
        for j in range(size):
            npt.assert_equal(mat_tile[i, j], mat_mkl[i, j])

def check_performance():

    setup = '''
import _matrix as m
SIZE = 1000 ## for performance test
mat1 = m.Matrix(SIZE, SIZE)
mat2 = m.Matrix(SIZE, SIZE)
for i in range(SIZE* SIZE):
    mat1[i] = i + 1
    mat2[i] = i + 1
'''

    naive = timeit.Timer('m.multiply_naive(mat1, mat2)', setup=setup)
    tile = timeit.Timer('m.multiply_tile(mat1, mat2, 8).multiply()', setup=setup)

    repeat = 5

    naivesec = min(naive.repeat(repeat=repeat, number=1))
    tilesec = min(tile.repeat(repeat=repeat, number=1))

    with open('performance.txt', 'w') as f:
        f.write("matrix multiplication performance\n")
        f.write("Take best score among {} trials\n".format(repeat))
        f.write("naive method : {} seconds\n".format(naivesec))
        f.write("tile method  : {} seconds\n".format(tilesec))
        f.write("tile method is {:.2f}% " \
                "faster than naive one\n".format(100*(1-(tilesec/naivesec))))

if __name__ == "__main__":
    check_performance()
