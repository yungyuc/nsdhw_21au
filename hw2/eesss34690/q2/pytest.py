import _vector as vec
import pytest

def t1_null_1():
    with pytest.raises(invalid_argument):
        vec.get_Angle(0,0,3,7)

def t2_null_2():
    with pytest.raises(invalid_argument):
        vec.get_Angle(4,10,0,0)

def t3_right_90():
    ans = vec.get_Angle(0,4,9,0)
    assert ans == 90

def t4_other():
    ans = vec.get_Angle(0,0,1,1)
    assert ans == 45

