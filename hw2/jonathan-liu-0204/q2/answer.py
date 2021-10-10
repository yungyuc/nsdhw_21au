from _vector import *
import pytest
import math

def test_zero_length():
    vector0 = [0, 0]
    vector1 = [3, 8]
    result = get_angle(vector0[0], vector0[1], vector1[0], vector1[1])
    assert math.isclose(get_angle(vector0[0], vector0[1], vector1[0], vector1[1]), -1)
    
def test_zero_angle():
    vector0 = [1, 1]
    vector1 = [5, 5]
    result = get_angle(vector0[0], vector0[1], vector1[0], vector1[1])
    assert math.isclose(get_angle(vector0[0], vector0[1], vector1[0], vector1[1]), 0)

def test_right_angle(): #90
    vector0 = [0, 3]
    vector1 = [5, 0]
    result = get_angle(vector0[0], vector0[1], vector1[0], vector1[1])
    assert math.isclose(get_angle(vector0[0], vector0[1], vector1[0], vector1[1]), math.pi/2)

def test_flat_angle(): #180
    vector0 = [6, 0]
    vector1 = [-4, 0]
    result = get_angle(vector0[0], vector0[1], vector1[0], vector1[1])
    assert math.isclose(get_angle(vector0[0], vector0[1], vector1[0], vector1[1]), math.pi)