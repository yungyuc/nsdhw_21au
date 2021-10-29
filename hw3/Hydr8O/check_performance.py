import _matrix
import timeit

size = 1000

mat1 = _matrix.Matrix(size,size)
mat2 = _matrix.Matrix(size,size)

ns = dict(_matrix=_matrix, mat1=mat1, mat2=mat2)
t_tile = timeit.Timer("_matrix.multiply_tile(mat1, mat2, 16)", globals=ns)
t_naive = timeit.Timer('_matrix.multiply_naive(mat1, mat2)', globals=ns)


time_tile = min(t_tile.repeat(10, 1))
time_naive = min(t_naive.repeat(10, 1))
ratio = time_tile/time_naive

with open('performance.txt', 'w') as performance:
    performance.write(f'Naive time: {time_naive.elapsed()}\n')
    performance.write(f'Tiling time: {time_tile.elapsed()}\n')
    performance.write(f'Speed up: {ratio}\n')