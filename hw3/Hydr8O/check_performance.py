import _matrix

size = 2000

mat1 = _matrix.Matrix(size,size)
mat2 = _matrix.Matrix(size,size)
ret_naive = _matrix.multiply_naive(mat1, mat2)
ret_tile = _matrix.multiply_tile(mat1, mat2, 16)

with open('performance.txt', 'w') as performance:
    performance.write(f'Naive time: {ret_naive.elapsed()}\n')
    performance.write(f'Tiling time: {ret_tile.elapsed()}\n')
    performance.write(f'Speed up: {ret_naive.elapsed() / ret_tile.elapsed()}\n')