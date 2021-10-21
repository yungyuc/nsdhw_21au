from _vector import *
import math

def test_angle_zero():
    a = myvector(1, 0)
    b = myvector(1, 0)
    assert math.isclose(angle(a, b), 0)

def test_angle_180():
    a = myvector(1, 0)
    b = myvector(-1, 0)
    assert math.isclose(angle(a, b), math.pi)

def test_angle_90():
    a = myvector(1, 0)
    b = myvector(0, 1)
    assert math.isclose(angle(a, b), math.pi / 2)

def test_angle_45():
    a = myvector(1, 0)
    b = myvector(1, 1)
    assert math.isclose(angle(a, b), math.pi / 4)
