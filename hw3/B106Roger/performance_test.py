import _matrix
import math
import timeit

def init_matrix(mat):
    r=mat.nrow
    c=mat.ncol
    for i in range(r):
        for j in range(c):
            mat[i,j]=i*mat.ncol+j

class TestClass:
    def test_mats(self, t1_row, t1_col, t2_row, t2_col):
        mat1=_matrix.Matrix(t1_row, t1_col)
        mat2=_matrix.Matrix(t2_row, t2_col)
        init_matrix(mat1)
        init_matrix(mat2)
        return mat1, mat2
    
    def test_speed(self,test_func, mat1, mat2, block_size=None):
        if block_size is None:
            test_func(mat1, mat2)
        else:
            test_func(mat1, mat2, block_size)

if __name__=='__main__':
    number=4
    bk_size=32
    r=1024
    c=1024

    test=TestClass()
    mat1, mat2=test.test_mats(r, c, r, c)

    f=open('performance.txt', 'w')
    t1=timeit.timeit(lambda: test.test_speed(_matrix.multiply_naive, mat1, mat2), number=number)
    str1=f'The execution time of _matrix.multiply_naive between two {r}x{c} matrices is {t1/number}\n'
    print(str1,end='')
    t2=timeit.timeit(lambda: test.test_speed(_matrix.multiply_tile, mat1, mat2, bk_size), number=number)
    str2=f'The execution time of _matrix.multiply_tile {bk_size} between two {r}x{c} matrices is {t2/number}\n'
    print(str2,end='')
    t3=timeit.timeit(lambda: test.test_speed(_matrix.multiply_mkl, mat1, mat2), number=number)
    str3=f'The execution time of _matrix.multiply_mkl between two {r}x{c} matrices is {t3/number}\n'
    print(str3)
    f.writelines([str1])
    f.writelines([str2])
    f.writelines([str3])
    f.close()

    
        