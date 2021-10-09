import pytest
from _vector import *

def test_excepts1():
    assert calculate_angle([0],[0]) == -1
        
def test_excepts2():
    assert calculate_angle([], []) == -1
        
def test_excepts3():
    assert calculate_angle([1], [2,4]) == -1
        
def test_valid1():
    calculate_angle([1,2], [2,4]) == pytest.approx(0)

def test_valid2():
    calculate_angle([1,0], [0,1]) == pytest.approx(1.57)

def test_valid3():
    calculate_angle([2,5], [2,3]) == pytest.approx(0.20)
