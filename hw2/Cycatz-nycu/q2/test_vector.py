#!/usr/bin/env python3

import _vector
import math
import pytest
from random import random, randint, uniform

EPS = 1e-4
NUM_TESTS = 10000
MAX_RADIUS = 10000

def generate_testcases(dtheta, radius):
    if dtheta is None:
        dtheta = uniform(-180, 180)
    theta_u = randint(0, 360)
    theta_v = theta_u + dtheta
    radians = math.radians(theta_u), math.radians(theta_v)

    if radius is None:
        radius = randint(1, MAX_RADIUS), randint(1, MAX_RADIUS)

    u = [math.cos(radians[0]) * radius[0], math.sin(radians[0]) * radius[0]]
    v = [math.cos(radians[1]) * radius[1], math.sin(radians[1]) * radius[1]]

    return u, v, math.radians(dtheta)

def calc_angle(u, v):
    cross = u[0] * v[1] - u[1] * v[0]
    dot = u[0] * v[0] + u[1] * v[1]
    return math.atan2(cross, dot)


# Test for empty 2-vector (invalid input).
def test_empty_vector():
    with pytest.raises(ValueError):
        _vector.calc([], [])
    with pytest.raises(ValueError):
        _vector.calc([1], [1])


# Test for zero angle.
def test_zero_angle():
    for _ in range(1, NUM_TESTS):
        u, v, ans = generate_testcases(0, None)
        res = _vector.calc(u, v)
        assert abs(res - ans) < EPS

    for _ in range(1, NUM_TESTS):
        u, v, ans = generate_testcases(-0, None)
        res = _vector.calc(u, v)
        assert abs(res - ans) < EPS

# Test for zero length. (invalid input)
def test_zero_length():
    for _ in range(1, NUM_TESTS):
        u, v, ans = generate_testcases(None, (0, randint(1, NUM_TESTS)))
        with pytest.raises(ValueError):
            res = _vector.calc(u, v)

    for _ in range(1, NUM_TESTS):
        u, v, ans = generate_testcases(None, (randint(1, NUM_TESTS), 0))
        with pytest.raises(ValueError):
            res = _vector.calc(u, v)

# Test for right angle (90-deg).
def test_right_angle():
    for i in range(1, NUM_TESTS):
        u, v, ans = generate_testcases(90, None)
        res = _vector.calc(u, v)
        assert abs(res - ans) < EPS

    for i in range(1, NUM_TESTS):
        u, v, ans = generate_testcases(-90, None)
        res = _vector.calc(u, v)
        assert abs(res - ans) < EPS

        
# Test for 180-degree angle

# Note:
# we need to calculate the angle in Python
# with given u and v to deal with the precision issue

def test_180_angle():
    for i in range(1, NUM_TESTS):
        u, v, ans = generate_testcases(180, None)
        res = _vector.calc(u, v)
        assert abs(res - ans) < EPS or abs(res - calc_angle(u, v)) < EPS

    for i in range(1, NUM_TESTS):
        u, v, ans = generate_testcases(-180, None)
        res = _vector.calc(u, v)
        assert abs(res - ans) < EPS or abs(res - calc_angle(u, v)) < EPS

# Test for other angles
def test_other_angle():
    for i in range(1, NUM_TESTS):
        u, v, ans = generate_testcases(None, None)
        res = _vector.calc(u, v)
        assert abs(res - ans) < EPS

# Test for invalid inputs, e.g., inf, nan
def test_invalid_inputs():
    possible_vals = [float('inf'), float('nan'), None]
    for val1 in possible_vals:
        for val2 in possible_vals:
            for val3 in possible_vals:
                for val4 in possible_vals:
                    if not (val1, val2, val3, val4) == (None, None, None, None):
                        val1_p, val2_p, val3_p, val4_p = val1, val2, val3, val4
                        if val1_p == None:
                            val1_p = random() * 10000
                        if val2_p == None:
                            val2_p = random() * 10000
                        if val3_p == None:
                            val3_p = random() * 10000
                        if val4_p == None:
                            val4_p = random() * 10000
                        with pytest.raises(ValueError):
                            _vector.calc([val1_p, val2_p], [val3_p, val4_p])
