#!/usr/bin/env python3

import _vector

def test_1():
    x = _vector.getAngle([0.1, -13, 1.116], [-20, 15.214, -33.125])
    assert round(x, 2) == 2.02

def test_2():
    x = _vector.getAngle([0.003, 0.0219], [-0.155, 0.3])
    assert round(x, 2) == 0.61

def test_3():
    x = _vector.getAngle([111, 98, -12], [-11, 1000, -51.1])
    assert round(x, 2) == 0.86

def test_4():
    x = _vector.getAngle([0.0001, -0.0001], [0.0001, -0.0001])
    assert round(x, 2) == 0.00

def test_5():
    x = _vector.getAngle([1000.11, 0.0005], [9999.99, 6666.66])
    assert round(x, 2) == 0.59

def test_6():
    x = _vector.getAngle([1, -1], [-1, 1])
    assert round(x, 2) == 3.14

def test_7():
    x = _vector.getAngle([12345, 12345], [24690, 24690])
    assert round(x, 2) == 0.00