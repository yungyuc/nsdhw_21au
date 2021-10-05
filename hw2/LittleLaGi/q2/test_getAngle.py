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
