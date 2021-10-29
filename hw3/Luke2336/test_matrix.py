import _matrix
import numpy as np
import time
import pytest


def test_multiply_naive():
  row1 = 100
  col1 = 100
  row2 = col1
  col2 = 100
  buffer1 = np.random.random(row1 * col1)
  buffer2 = np.random.random(row2 * col2)
  mat1 = buffer1.reshape(row1, col1)
  mat2 = buffer2.reshape(row2, col2)
  mat3 = np.dot(mat1, mat2)
  m1 = _matrix.Matrix(mat1)
  m2 = _matrix.Matrix(mat2)
  ret = _matrix.multiply_naive(m1, m2)

  for i in range(row1):
    for j in range(col2):
      assert (mat3[i, j] - ret[i, j] < 1e-4) and (ret[i, j] - mat3[i, j] < 1e-4)


def test_multiply_tile():
  row1 = 100
  col1 = 100
  row2 = col1
  col2 = 100
  buffer1 = np.random.random(row1 * col1)
  buffer2 = np.random.random(row2 * col2)
  mat1 = buffer1.reshape(row1, col1)
  mat2 = buffer2.reshape(row2, col2)
  mat3 = np.dot(mat1, mat2)
  m1 = _matrix.Matrix(mat1)
  m2 = _matrix.Matrix(mat2)
  ret = _matrix.multiply_tile(m1, m2, 4)

  for i in range(row1):
    for j in range(col2):
      assert (mat3[i, j] - ret[i, j] < 1e-4) and (ret[i, j] - mat3[i, j] < 1e-4)


def test_time():
  size = 1000
  ma = _matrix.Matrix(size, size)
  mb = _matrix.Matrix(size, size)
  for i in range(size):
    for j in range(size):
      ma[i, j] = i + j
      mb[i, j] = i * j

  start_time = time.time()
  for i in range(5):
    _matrix.multiply_naive(ma, mb)
  end_time = time.time()
  time_naive = (end_time - start_time) / 5

  start_time = time.time()
  for i in range(5):
    _matrix.multiply_tile(ma, mb, 32)
  end_time = time.time()
  time_tile = (end_time - start_time) / 5

  file = open('performance.txt', 'w')
  file.write('naive:' + str(time_naive) + '\n')
  file.write('tile:' + str(time_tile) + '\n')
  file.close()

  assert time_tile <= time_naive * 0.8
  