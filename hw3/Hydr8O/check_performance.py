import _matrix

ret_naive = _matrix.multiply_naive(mat1, mat2)
ret_tile = _matrix.multiply_tile(mat1, mat2, 64)

with open('performance.txt', 'w') as performance:
    performance.write(f'Naive time: {ret_naive.elapsed()}\n')
    performance.write(f'Tiling time: {ret_tile.elapsed()}\n')
    performance.write(f'Speed up: {ret_native.elapsed() / ret_tile.elapsed()}\n')