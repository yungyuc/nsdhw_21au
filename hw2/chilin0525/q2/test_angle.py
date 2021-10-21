import _vector
import math
import pytest

fixed_num = 10000000
def calc_vector_angle(x1,y1,x2,y2):
    _v1 = _vector.create_vector(x1,y1)
    _v2 = _vector.create_vector(x2,y2)
    _ans= _vector.calc(_v1,_v2)
    return _ans


def test_zero_length1():
    try: 
        res = int(calc_vector_angle(0, 0, 1, 0))
    except ValueError:
        res = None
    assert res==None


def test_zero_length2():
    try: 
        res = int(calc_vector_angle(0, 0, 0, 0))
    except ValueError:
        res = None
    assert res==None


def test_zero_angle():
    res = int(calc_vector_angle(5, 0, 0, 3)*fixed_num)
    ans = int(math.acos(0.0)*fixed_num)
    assert res==ans


def test_right_angle():
    res = int(calc_vector_angle(1, 0, 0, 5))*fixed_num
    ans = int(math.pi/2)*fixed_num
    assert res==ans


# mul 10000000 to get 7 length after decimal point
def test_other_ans1():
    res = int(calc_vector_angle(4, 5, 18, 9)*fixed_num)
    ans = int(0.43240777557053783*fixed_num)
    assert res==ans


def test_other_ans2():
    res = int(calc_vector_angle(4.3, 7.9, 1.5, 11.7)*fixed_num)
    ans = int(0.370949852251141*fixed_num)
    assert res==ans