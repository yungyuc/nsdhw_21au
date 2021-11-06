import pytest
import _matrix as M
import numpy as np
import timeit


def test_int_multiply():
    A = M.Matrix(5,10)
    a_value = np.random.randint(100,size=(5,10))
    A.load(a_value)

    B = M.Matrix(10,3)
    b_value = np.random.randint(100,size=(10,3))
    B.load(b_value)

    result_numpy = M.Matrix(5,3)
    result_numpy.load(np.dot(a_value,b_value))
    result_naive = M.multiply_naive(A,B)
    result_tile = M.multiply_tile(A,B,8)
    result_mkl = M.multiply_mkl(A,B)

    assert(result_numpy==result_naive)
    assert(result_numpy==result_tile)
    assert(result_numpy==result_mkl)

def test_performance():
    setup_code = '''
import _matrix as M
import numpy as np
A = M.Matrix(256,512)
a_value = np.random.randint(1000,size=(256,512))
A.load(a_value)
B = M.Matrix(512,128)
b_value = np.random.randint(1000,size=(512,128))
B.load(b_value)
'''
    naive_mul_time = timeit.timeit(stmt='M.multiply_naive(A,B)',setup=setup_code, number=100)
    tile_mul_time = timeit.timeit(stmt='M.multiply_tile(A,B,16)',setup=setup_code, number=100)
    mkl_mul_time = timeit.timeit(stmt='M.multiply_mkl(A,B)',setup=setup_code, number=100)
    with open("performance.txt","w") as f:
    	f.write(f"Matrix multiplication A(256,512)*B(512,128) 100 times using naive method takes {naive_mul_time:.4f} secs")
    	f.write(f"Matrix multiplication A(256,512)*B(512,128) 100 times using tileing method takes {tile_mul_time:.4f} secs")
    	f.write(f"Matrix multiplication A(256,512)*B(512,128) 100 times using mkl method takes {mkl_mul_time:.4f} secs")
