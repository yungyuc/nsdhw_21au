#!/usr/bin/env python3

import timeit

NUMBER = 1
REPEAT = 5
SETUP = '''
import random

from _matrix import Matrix, multiply_naive, multiply_tile, multiply_mkl

M = 1000
N = 1000
K = 1000

mat1 = Matrix(M, K)
for i in range(M):
    for j in range(K):
        mat1[i, j] = random.random()

mat2 = Matrix(K, N)
for i in range(K):
    for j in range(N):
        mat2[i, j] = random.random()
'''
BENCHSETS = [(f'Tile (size {tile_size})',
              f'multiply_tile(mat1, mat2, {tile_size})')
             for tile_size in map(lambda x: 2**x, range(8))] \
    + [('MKL', 'multiply_mkl(mat1, mat2)')]


def mean(a: list[float]) -> float:
    return sum(a) / len(a)


def print_header() -> None:
    print('Flavour', 'Time (s)', 'Speedup', sep='\t')


def print_row(flavour, time, speedup) -> None:
    print(flavour, f'{time:.4g}', f'{speedup:.4g}', sep='\t')


def main():
    print_header()

    naive_time = mean(timeit.repeat(
        'multiply_naive(mat1, mat2)', SETUP, number=NUMBER, repeat=REPEAT))
    print_row('Naive', naive_time, 1)

    for flavour, stmt in BENCHSETS:
        time = mean(timeit.repeat(stmt, SETUP, number=NUMBER, repeat=REPEAT))
        print_row(flavour, time, naive_time / time)


if __name__ == '__main__':
    main()
