import cal_vector
import numpy as np
import math
import pytest


def test_angle_two_vec_random():
    vectorA = np.random.uniform(-100, 100, [2])
    vectorB = np.random.uniform(-100, 100, [2])
    dotProduct = np.dot(vectorA, vectorB)
    normvectorA = np.linalg.norm(vectorA)
    normvectorB = np.linalg.norm(vectorB)
    costheta = dotProduct / (normvectorA * normvectorB)
    angle = np.arccos(costheta)

    tested_angle = cal_vector.angle_two_vec(vectorA, vectorB)

    assert math.isclose(angle, tested_angle, rel_tol=1e-03)


def test_angle_two_vec_right_angle():
    vectorA = [np.random.uniform(-100, 100), 0.0]
    vectorB = [0.0, np.random.uniform(-100, 100)]

    tested_angle = cal_vector.angle_two_vec(vectorA, vectorB)

    assert math.isclose(math.radians(90), tested_angle, rel_tol=1e-03)


def test_angle_two_vec_zero_angle():
    vectorA = [np.random.uniform(0, 100), 0.0]
    vectorB = [np.random.uniform(0, 100), 0.0]

    tested_angle = cal_vector.angle_two_vec(vectorA, vectorB)

    assert math.isclose(0.0, tested_angle, rel_tol=1e-03)


def test_angle_two_vec_invaild():
    vectorA = []
    vectorB = []

    with pytest.raises(ValueError):
        cal_vector.angle_two_vec(vectorA, vectorB)
