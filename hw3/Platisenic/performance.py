import timeit

setup = '''
import _matrix
size = 1000
mat1 = _matrix.Matrix(size,size)
mat2 = _matrix.Matrix(size,size)

for it in range(size):
    for jt in range(size):
        mat1[it, jt] = it * size + jt + 1
        mat2[it, jt] = it * size + jt + 1
'''

naive = timeit.Timer('_matrix.multiply_naive(mat1, mat2)', setup=setup)
tile = timeit.Timer('_matrix.multiply_tile(mat1, mat2, 50)', setup=setup)
mkl = timeit.Timer('_matrix.multiply_mkl(mat1, mat2)', setup=setup)


with open('performance.txt', 'w') as fobj:

    fobj.write(f'Start multiply_naive, take ')
    naivesec = minsec = naive.timeit(1)
    fobj.write(f'{minsec} seconds\n')

    fobj.write(f'Start multiply_mkl, take ')
    mklsec = minsec = mkl.timeit(1)
    fobj.write(f'{minsec} seconds\n')

    fobj.write('MKL speed-up over naive: %g x\n' % (naivesec/mklsec))

    fobj.write(f'Start multiply_tile, take min ')
    tilesec = minsec = tile.timeit(1)
    fobj.write(f'{minsec} seconds\n')

    fobj.write('Tile speed-up over naive: %g x\n' % (naivesec/tilesec))
