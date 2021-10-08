import _vector
import numpy as np
import pytest
import math

class Test_Radians:
    abs_error = 0.00001

    def test_invalid(self):
        v1 = np.array([0, 0])
        v2 = np.random.randint(1,100,2) 
        result = _vector.angle(v1, v2)

        assert result == -1

    def test_zero_angle(self):
        coefficient = np.random.randint(1,100)
        v1 = np.random.randint(1,100,2) 
        v2 = coefficient * v1
        result = _vector.angle(v1, v2)

        assert abs(result - 0) < self.abs_error

    def test_45_angle(self):
        v1 = np.array([math.sqrt(2), 0])
        v2 = np.array([1, 1])
        result = _vector.angle(v1, v2)

        assert abs(result - math.pi/4) < self.abs_error

    def test_right_angle(self):
        v1 = np.array([1, 0])
        v2 = np.array([0, 1])
        result = _vector.angle(v1, v2)

        assert abs(result - math.pi/2) < self.abs_error

    def test_180_angle(self):
        v1 = np.random.randint(1,100,2) 
        v2 = np.array([-v1[0], -v1[1]])
        result = _vector.angle(v1, v2)

        assert abs(result - math.pi) < self.abs_error

