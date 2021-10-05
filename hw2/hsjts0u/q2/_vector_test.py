from _vector import *
import math

pi = 3.14159265358979323846

class Test_vector:
    
    def test_zeroLength(self):
        v1 = vec(0, 0)
        v2 = vec(0, 0)
        assert math.isclose(calc(v1, v2), -1.0)
    
    def test_zeroAngle(self):
        v1 = vec(1, 0)
        v2 = vec(1, 0)
        assert math.isclose(calc(v1, v2), 0)
    
    def test_rightAngle(self):
        v1 = vec(1, 0)
        v2 = vec(0, 1)
        assert math.isclose(calc(v1, v2), pi/2)

    def test_otherAngle(self):
        v1 = vec(-1, 0)
        v2 = vec(1, 0)
        assert math.isclose(calc(v1, v2), pi)

