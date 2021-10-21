import _vector
import math

def test_1():
    assert(_vector.angle(0,0,0,0) == 0.0)

def test_2():
    assert(_vector.angle(12.9,15.8,12.9,15.8) == 0.0)

def test_3():
    program_value = _vector.angle(0,1,1,0)
    test_ans = math.pi / 2.0
    assert(abs(program_value-test_ans) < 1e-6)

def test_4():
    program_value = _vector.angle(0,1,0,-1)
    test_ans = math.pi
    assert(abs(program_value-test_ans) < 1e-6)

def test_5():
    program_value = _vector.angle(1,0,1,math.sqrt(3))
    test_ans = math.pi / 3
    assert(abs(program_value-test_ans) < 1e-6)