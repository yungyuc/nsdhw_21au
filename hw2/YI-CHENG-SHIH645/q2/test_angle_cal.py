import math

from _vector import *


def test_right_angle():
    # cos 90度 = 0
    # arc cos (0) ~ 1.571
    radian_angle = angle(Vector2D(1, 0), Vector2D(0, 1))
    assert round(radian_angle, 3) == 1.571


def test_zero_angle():
    # cos 0度 = 1
    # arc cos (1) = 0
    radian_angle = angle(Vector2D(1, 1), Vector2D(2, 2))
    assert radian_angle == 0
