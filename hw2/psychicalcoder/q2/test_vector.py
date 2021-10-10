import pytest
import _vector
import math
from math import cos, sin
from random import uniform

CASES = 1000

def ref_ans(v1, v2):
    dot = v1[0] * v2[0] + v1[1] * v2[1];
    l1 = math.hypot(v1[0], v1[1])
    l2 = math.hypot(v2[0], v2[1])
    return math.acos(dot/l1/l2)

def gen_two_vec(angle):
    v1 = [uniform(-1000.0, 1000.0), uniform(-1000.0, 1000.0)]
    scale = uniform(0.1, 100)
    cosa = cos(angle)
    sina = sin(angle)
    v2 = [scale * (cosa * v1[0] - sina * v1[1]), scale * (sina * v1[0] + cosa * v1[1])]
    return v1, v2

def test_zero_length_vector():
    zerovec1 = [0.0, 0.0]
    zerovec2 = [0.0, 0.0]
    for _ in range(0, CASES):
        v1 = [uniform(-1000.0, 1000.0), uniform(-1000.0, 1000.0)]
        with pytest.raises(ValueError):
            _vector.angle(zerovec1, v1)
        with pytest.raises(ValueError):
            _vector.angle(v1, zerovec2)
    with pytest.raises(ValueError):
        _vector.angle(zerovec1, zerovec2)

def test_right_angle():
    for _ in range(0, CASES):
        v1, v2 = gen_two_vec(math.radians(90))
        # print(v1, v2)
        # print("ref" , ref_ans(v1, v2), "ret", _vector.angle(v1, v2))
        assert math.isclose(ref_ans(v1, v2), _vector.angle(v1, v2), abs_tol=1e-7)
    for _ in range(0, CASES):
        v1, v2 = gen_two_vec(math.radians(-90))
        assert math.isclose(ref_ans(v1, v2), _vector.angle(v1, v2), abs_tol=1e-7)

def test_non2d_vector():
    with pytest.raises(ValueError):
        _vector.angle([],[])
    with pytest.raises(ValueError):
        _vector.angle([1],[2, 1])
    with pytest.raises(ValueError):
        _vector.angle([2, 1],[1]) 
    with pytest.raises(ValueError):
        _vector.angle([], [1, 1])
    with pytest.raises(ValueError):
        _vector.angle([1, 1], [])
    with pytest.raises(ValueError):
        _vector.angle([1, 1, 1], [2, 1])
    with pytest.raises(ValueError):
        _vector.angle([2, 1],[1, 1, 1])

def test_other_angle():
    for _ in range(CASES):
        angle = uniform(-2.0*math.pi, 2.0*math.pi)
        v1, v2 = gen_two_vec(angle)
        ret = _vector.angle(v1, v2)
        assert math.isclose(ref_ans(v1, v2), ret, abs_tol=1e-7)

def test_zero_angle():
    for _ in range(CASES):
        v1, v2 = gen_two_vec(0.0)
        ret = _vector.angle(v1, v2)
        assert math.isclose(ret, 0.0, abs_tol=1e-7)

def test_180_angle():
    for _ in range(CASES):
        v1, v2 = gen_two_vec(math.pi)
        ret = _vector.angle(v1, v2)
        assert math.isclose(math.pi, ret, abs_tol=1e-7)
    for _ in range(CASES):
        v1, v2 = gen_two_vec(-math.pi)
        ret = _vector.angle(v1, v2)
        assert math.isclose(math.pi, ret, abs_tol=1e-7)

def test_ultralong_vector():
    with pytest.raises(ValueError):
        _vector.angle([1e308, 1e308], [4, 3])
    with pytest.raises(ValueError):
        _vector.angle([4, 3], [1e308, 1e308])
    with pytest.raises(ValueError):
        _vector.angle([1e308, 1e308], [1e308, 1e308])
