from _vector import _vector
import math

def test_ZeroAngle():
    assert _vector(1, 1, 1, 1) == 0

def test_90Angle():
    angle = _vector(1, 0, 0, 1) * 180 / math.pi
    assert round(angle, 1) == 90
