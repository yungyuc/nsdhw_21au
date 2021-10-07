from _vector import calc_angle
import pytest
import math


def test_zero_length():
    with pytest.raises(ValueError):
        calc_angle([0.0, 0.0], [0.0, 0.0])


def test_zero_angle():
    assert calc_angle([1.0, 1.0], [10.0, 10.0]) == pytest.approx(0.0, abs=1e-6)


def test_right_angle():
    assert calc_angle([1.0, 0.0], [0.0, 1.0]) == pytest.approx(math.pi / 2.0)
    assert calc_angle([0.0, 1.0], [1.0, 0.0]) == pytest.approx(math.pi / 2.0)


def test_other_angle():
    assert calc_angle([1.0, 0.0], [1.0, 1.0]) == pytest.approx(math.pi / 4.0)
    assert calc_angle([0.0, 1.0], [1.0, 1.0]) == pytest.approx(math.pi / 4.0)


