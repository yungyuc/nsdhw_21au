import math
import pytest
from _vector import calculate_angle


def test_zero_length():
    try:
        calculate_angle([0, 0], [1, 1])
    except ValueError:
        assert True


def test_right_angle():
    assert calculate_angle([1, 0], [0, 1]) == math.radians(90)

