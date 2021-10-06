from _vector import _vector
import math

def test_zero_angle():
    assert _vector([20, 20], [20, 20]) == 0

def test_right_angle():
    angle = _vector([20, 0], [0,20]) * 180 / math.pi
    assert round(angle, 5) == 90

def test_180_angle():
    angle = _vector([0, 5], [0, -5])
    assert round(angle, 5) == round(math.pi, 5)
