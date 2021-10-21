import _vector
import pytest
import math
import random
import numpy as np

abs_val = 0.00001

class TestGroup:

    def test_invalid_input(self):
        v1 = [0,0]
        v2 = [random.randrange(1,50,1) for i in range(2)]
        result = _vector.cal_angle(v1,v2)
        assert result == -666

    def test_zero_angle(self):
        v1 = [random.randrange(1,50,1) for i in range(2)]
        v2 =  v1
        result = _vector.cal_angle(v1,v2)
        assert result == 0

    def test_right_angle(self):
        v1 = [random.randrange(1,50,1) for i in range(2)]
        v2 = []
        v2.append(-v1[1])
        v2.append(v1[0])
        result = _vector.cal_angle(v1,v2)
        assert abs((result - math.pi/2)) <= abs_val

    def test_random_angle(self):
        v1 = [random.randrange(1,50,1) for i in range(2)]
        v2 = [random.randrange(1,50,1) for i in range(2)]

        result = _vector.cal_angle(v1,v2)
        v1_unit = v1 / np.linalg.norm(v1)
        v2_unit = v2 / np.linalg.norm(v2)
        dot_product = np.dot(v1_unit,v2_unit)
        ans = np.arccos(dot_product)
        assert abs((result - ans)) <= abs_val
