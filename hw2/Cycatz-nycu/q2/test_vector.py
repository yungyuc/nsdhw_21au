import _vector
import math
import pytest


EPS = 1e-4

# Test for zero-length 2-vector (invalid input).
def test_zero_length_vector():
    with pytest.raises(ValueError):
        _vector.calc([], [])

# Test for zero angle.
def test_zero_angle():
    assert _vector.calc([1, 1], [1, 1]) == 0.0

# Test for right angle (90-deg).
def test_right_angle():
    assert abs(_vector.calc([1, 0], [0, 1]) - 90 / 360 * 2 * math.pi) < EPS

# Test for other angles
def test_30_degree_angle():
    assert abs(_vector.calc([1, 0], [math.sqrt(3), 1]) - 30 / 360 * 2 * math.pi) < EPS

def test_45_degree_angle():
    assert abs(_vector.calc([1, 0], [math.sqrt(2), math.sqrt(2)]) - 45 / 360 * 2 * math.pi) < EPS

def test_60_degree_angle():
    assert abs(_vector.calc([1, 0], [1, math.sqrt(3)]) - 60 / 360 * 2 * math.pi) < EPS

def test_180_degree_vector():
    assert abs(_vector.calc([1, 0], [-1, 0]) - 180 / 360 * 2 * math.pi) < EPS
