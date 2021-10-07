#!/usr/bin/env python3

import math
import pytest
from vecAngle import angle_between

def test_zero_length():
    v1 = [0, 0]
    v2 = [2, 3]
    cpp_result = angle_between(v1[0], v1[1], v2[0], v2[1])
    # If input is invalid, the function should return a value that is not in the range of acos(x)
    assert cpp_result < 0 or cpp_result > math.pi

def test_zero_angle():
    v1 = [1, 1]
    v2 = [3, 3]
    cpp_result = angle_between(v1[0], v1[1], v2[0], v2[1])
    ans = 0.0
    assert abs(cpp_result-ans) < 1e-4

def test_right_angle():
    v1 = [1, 2]
    v2 = [-2, 1]
    cpp_result = angle_between(v1[0], v1[1], v2[0], v2[1])
    ans = math.pi / 2
    assert abs(cpp_result-ans) < 1e-4

def test_other_angle():
    v1 = [3, math.sqrt(3)]
    v2 = [-math.sqrt(3), 1]
    cpp_result = angle_between(v1[0], v1[1], v2[0], v2[1])
    ans = math.pi * 2/3
    assert abs(cpp_result-ans) < 1e-4

