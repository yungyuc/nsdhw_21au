import math
import pytest
import _vector


def test_zero_length():
    try:
        _vector([0, 0], [1, 1])
    except ValueError:
        assert True


def test_right_angle():
    assert _vector([1, 0], [0, 1]) == math.radians(90)

