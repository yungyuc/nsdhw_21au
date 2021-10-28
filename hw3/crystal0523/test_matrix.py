import _matrix
import timeit
import unittest

class Test_matrix(unittest.TestCase):

    def make_matrices(self, size):
        mat1 = _matrix.Matrix(size,size)
        mat2 = _matrix.Matrix(size,size)
        mat3 = _matrix.Matrix(size,size)

        for it in range(size):
            for jt in range(size):
                mat1[it, jt] = it * size + jt + 1
                mat2[it, jt] = it * size + jt + 1
                mat3[it, jt] = 0

        return mat1, mat2, mat3


    def test_basic(self):
        size = 100
        mat1, mat2, mat3, *_ = self.make_matrices(size)

        assert size == mat1.nrow()
        assert size == mat1.ncol()
        assert size == mat2.nrow()
        assert size == mat2.ncol()
        assert size == mat3.nrow()
        assert size == mat3.ncol()
       
        assert 2 == mat1[0,1]
        assert size+2 == mat1[1,1]
        assert size*2 == mat1[1,size-1]
        assert size*size == mat1[size-1,size-1]
      
        for i in range(mat1.nrow()):
            for j in range(mat1.ncol()):
                assert 0 != mat1[i,j]
                assert mat1[i,j] == mat2[i,j]
                assert 0 == mat3[i,j]
        assert mat1 == mat2
        assert mat1 is not mat2
       
    def test_match_naive_mkl(self):
        size = 100
        mat1, mat2, *_ = self.make_matrices(size)

        ret_naive = _matrix.multiply_naive(mat1, mat2)
        ret_mkl = _matrix.multiply_mkl(mat1, mat2)

        assert size == ret_naive.nrow()
        assert size == ret_naive.ncol()
        assert size == ret_mkl.nrow()
        assert size == ret_mkl.ncol()

        for i in range(ret_naive.nrow()):
            for j in range(ret_naive.ncol()):
                assert mat1[i,j] == ret_mkl[i,j]
                assert ret_naive[i,j] == ret_mkl[i,j]

    def test_zero(self):
        size = 100
        mat1, mat2, mat3, *_ = self.make_matrices(size)

        ret_naive = _matrix.multiply_naive(mat1, mat3)
        ret_mkl = _matrix.multiply_mkl(mat1, mat3)

        assert size == ret_naive.nrow()
        assert size == ret_naive.ncol()
        assert size == ret_mkl.nrow()
        assert size == ret_mkl.ncol()

        for i in range(ret_naive.nrow()):
            for j in range(ret_naive.ncol()):
                assert 0 == ret_naive[i,j]
                assert 0 == ret_mkl[i,j]

    def check_tile(self, mat1, mat2, tsize):
        if 0 == tsize:
            ret_tile = _matrix.multiply_naive(mat1, mat2)
            tile_str = "_matrix.multiply_naive(mat1, mat2)"
        else:
            ret_tile = _matrix.multiply_tile(mat1, mat2, tsize)
            tile_str = "_matrix.multiply_tile(mat1, mat2, tsize)"
        ret_mkl = _matrix.multiply_mkl(mat1, mat2)

        for i in range(ret_tile.nrow()):
            for j in range(ret_tile.ncol()):
                assert mat1[i,j] == ret_mkl[i,j]
                assert ret_tile[i,j] == ret_mkl[i,j]

        ns = dict(_matrix=_matrix, mat1=mat1, mat2=mat2, tsize=tsize)
        t_tile = timeit.Timer(tile_str, globals=ns)
        t_mkl = timeit.Timer('_matrix.multiply_mkl(mat1, mat2)', globals=ns)

        time_tile = min(t_tile.repeat(10, 1))
        time_mkl = min(t_mkl.repeat(10, 1))
        ratio = time_tile/time_mkl

        return ratio, time_tile

    def test_tile(self):

        mat1, mat2, *_ = self.make_matrices(500)

        ratio0, time0 = self.check_tile(mat1, mat2, 0)
        ratio16, time16 = self.check_tile(mat1, mat2, 16)
        ratio17, time17 = self.check_tile(mat1, mat2, 17)
        ratio19, time19 = self.check_tile(mat1, mat2, 19)
        if ratio16 / ratio0 < 0.8:
            assert False
        else:
            assert True

        if ratio17 / ratio0 < 0.8:
            assert False
        else:
            assert True

        if ratio19 / ratio0 < 0.8:
            assert False
        else:
            assert True
     
    def test_performance(self):
        size = 1000
        mat1, mat2, mat3, *_ = self.make_matrices(size)

        naive = timeit.Timer('_matrix.multiply_naive(mat1, mat2)')
        tile = timeit.Timer('_matrix.multiply_tile(mat1, mat2)')
        mkl = timeit.Timer('_matrix.multiply_mkl(mat1, mat2)')

        repeat = 1

        with open('performance.txt', 'w') as fobj:

            w = Writer([sys.stdout, fobj])

            w.write(f'Start multiply_naive (repeat={repeat}), take ')
            naivesec = naive.repeat(repeat=repeat, number=1)
            w.write(f'{naivesec} seconds\n')

            w.write(f'Start multiply_tile (repeat={repeat}), take ')
            tilesec = tile.repeat(repeat=repeat, number=1)
            w.write(f'{tilesec} seconds\n')

            w.write(f'Start multiply_mkl (repeat={repeat}), take ')
            mklsec = mkl.repeat(repeat=repeat, number=1)
            w.write(f'{mklsec} seconds\n')

            w.write('Tile speed-up over naive: %g x\n' % (naivesec/tilesec))
            w.write('MKL speed-up over naive: %g x\n' % (naivesec/mklsec))



       



