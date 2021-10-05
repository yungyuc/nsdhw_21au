import numpy as np
import math
import pytest
from vector import angle

class TestClass:
    def test_invalid(self):
        v1 = np.array([0, 0])
        v2 = np.random.uniform(-10.0, 10.0, 2)

        with pytest.raises(ValueError) as excinfo:
            angle(v1, v2)
        assert str(excinfo.value) == "zero-length 2-vector"

        with pytest.raises(ValueError) as excinfo:
            angle(v2, v1)
        assert str(excinfo.value) == "zero-length 2-vector"

    def test_zero(self):
        v1 = np.random.uniform(-10.0, 10.0, 2)
        v2 = v1 * np.random.uniform(0.1, 10.0)
        rad = angle(v1, v2)

        assert math.isclose(rad, 0, abs_tol = 1e-05)

    def test_right(self):
        v1 = (np.random.uniform(0.1, 10.0), 0.0)
        v2 = (0.0, np.random.uniform(0.1, 10.0))
        rad = angle(v1, v2)

        assert math.isclose(rad, math.pi / 2.0, abs_tol = 1e-05)

    def test_other(self):
        v1 = np.random.uniform(-10.0, 10.0, 2)
        v2 = np.random.uniform(-10.0, 10.0, 2)

        rad1 = angle(v1, v2)

        unit_v1 = v1 / np.linalg.norm(v1)
        unit_v2 = v2 / np.linalg.norm(v2)
        dot = np.dot(unit_v1, unit_v2)
        rad2 = np.arccos(dot)

        assert math.isclose(abs(rad1), rad2, abs_tol = 1e-05)

