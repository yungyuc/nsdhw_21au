import _matrix


def test_multiply_naive():
    size = 4
    mat1 = _matrix.Matrix(size,size)
    mat2 = _matrix.Matrix(size,size)
    for it in range(size):
        for jt in range(size):
            mat1[it, jt] = it * size + jt + 1
            mat2[it, jt] = it * size + jt + 1
    
    rst1 = _matrix.multiply_naive(mat1, mat2)
    rst2 = _matrix.multiply_mkl(mat1, mat2)

    for it in range(size):
        for jt in range(size):
            assert(rst1[it, jt] == rst2[it, jt])


def test_multiply_tile():
    size = 1000
    mat1 = _matrix.Matrix(size,size)
    mat2 = _matrix.Matrix(size,size)
    for it in range(size):
        for jt in range(size):
            mat1[it, jt] = it * size + jt + 1
            mat2[it, jt] = it * size + jt + 1
    
    rst1 = _matrix.multiply_tile(mat1, mat2, 50)
    rst2 = _matrix.multiply_mkl(mat1, mat2)
    
    for it in range(size):
        for jt in range(size):
            assert(rst1[it, jt] == rst2[it, jt])
