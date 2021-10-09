#!/usr/bin/env python3

import _vector
import math
import pytest

epsion = 1e-3

def test_zero_lenth():
    with pytest.raises(ValueError):
        _vector.getAngle([0,0],[1,1])

def test_zero_angle():
    x = _vector.getAngle([1,2],[3,6])
    # assert math.isclose(x,0.0,rel_tol=epsion)
    assert round(x, 3) == 0.0

def test_right_angle():
    x = _vector.getAngle([1,0],[0,2])
    # assert math.isclose(x,math.pi/2,rel_tol=epsion)
    assert round(x, 3) == 1.571

def test_other_angle():
    x = _vector.getAngle([1,2],[3,4])
    # assert math.isclose(x,0.17985,rel_tol=epsion)
    assert round(x, 3) == 0.180