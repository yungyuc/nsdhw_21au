import math
import random

from _vector import angle_between_r2

INFINITIES = (math.inf, -math.inf)


def random_nonzero_length() -> float:
    MEAN = 10

    while (r := random.expovariate(1 / MEAN)) == 0:
        pass
    return r


def random_angle() -> float:
    return random.uniform(-math.pi, math.pi)


def polar_to_cartesian(r: float, theta: float) -> tuple[float, float]:
    return (r * math.cos(theta), r * math.sin(theta))


def test_special() -> None:
    ABS_TOL = 1e-7

    assert abs(angle_between_r2((1, 0), (-1, 0)) - math.radians(180)) < ABS_TOL
    assert abs(angle_between_r2((1, 1), (-1, -1)) -
               math.radians(180)) < ABS_TOL

    assert abs(angle_between_r2((1, 0), (0, 1)) - math.radians(90)) < ABS_TOL
    assert abs(angle_between_r2((1, 1), (-1, 1)) - math.radians(90)) < ABS_TOL

    assert abs(angle_between_r2((1, 0), (1, 1)) - math.radians(45)) < ABS_TOL
    assert abs(angle_between_r2((1, 1), (0, 1)) - math.radians(45)) < ABS_TOL

    assert abs(angle_between_r2((1, 0), (1, math.sqrt(3))) -
               math.radians(60)) < ABS_TOL

    # Reference to a (famous?) math puzzle.
    assert abs(sum(angle_between_r2((1, 0), (1, x))
               for x in range(1, 4)) - math.radians(180)) < 3*ABS_TOL


def test_random() -> None:
    N_TEST = 10000
    ABS_TOL = 1e-7

    for _ in range(N_TEST):
        r1 = random_nonzero_length()
        r2 = random_nonzero_length()
        theta = random_angle()
        dtheta = random_angle()

        u = polar_to_cartesian(r1, theta)
        v = polar_to_cartesian(r2, theta + dtheta)

        assert abs(angle_between_r2(u, v) - abs(dtheta)) < ABS_TOL


def test_same() -> None:
    N_TEST = 10000
    ABS_TOL = 1e-7

    for _ in range(N_TEST):
        r = random_nonzero_length()
        theta = random_angle()

        v = polar_to_cartesian(r, theta)

        assert abs(angle_between_r2(v, v)) < ABS_TOL


def test_right_angle() -> None:
    N_TEST = 10000
    ABS_TOL = 1e-7

    for _ in range(N_TEST):
        r = random_nonzero_length()
        theta = random_angle()

        u = polar_to_cartesian(r, theta)
        x, y = u
        v1 = -y, x
        v2 = y, -x

        assert abs(angle_between_r2(u, v1) - math.radians(90)) < ABS_TOL
        assert abs(angle_between_r2(u, v2) - math.radians(90)) < ABS_TOL


def test_zero() -> None:
    N_TEST = 100

    for _ in range(N_TEST):
        r = random_nonzero_length()
        theta = random_angle()

        v = polar_to_cartesian(r, theta)

        assert math.isnan(angle_between_r2(v, (0, 0)))
        assert math.isnan(angle_between_r2((0, 0), v))


def test_both_zero() -> None:
    assert math.isnan(angle_between_r2((0, 0), (0, 0)))


def test_one_axis_inf() -> None:
    N_TEST = 100
    FINITE_AXIS_STDEV = 10
    ABS_TOL = 1e-7

    for _ in range(N_TEST):
        r = random_nonzero_length()
        theta = random_angle()
        finite_axis = random.normalvariate(0, FINITE_AXIS_STDEV)

        v = polar_to_cartesian(r, theta)

        for u1, u2 in (((math.inf, finite_axis), (1, 0)),
                       ((-math.inf, finite_axis), (-1, 0)),
                       ((finite_axis, math.inf), (0, 1)),
                       ((finite_axis, -math.inf), (0, -1))):
            assert abs(angle_between_r2(v, u1) -
                       angle_between_r2(v, u2)) < ABS_TOL
            assert abs(angle_between_r2(u1, v) -
                       angle_between_r2(u2, v)) < ABS_TOL


def test_both_axes_inf() -> None:
    N_TEST = 100

    for _ in range(N_TEST):
        r = random_nonzero_length()
        theta = random_angle()

        v = polar_to_cartesian(r, theta)

        for x in INFINITIES:
            for y in INFINITIES:
                assert math.isnan(angle_between_r2(v, (x, y)))
                assert math.isnan(angle_between_r2((x, y), v))


def test_both_one_axis_inf() -> None:
    N_TEST = 100
    FINITE_AXIS_STDEV = 10
    ABS_TOL = 1e-7

    for _ in range(N_TEST):
        f1 = random.normalvariate(0, FINITE_AXIS_STDEV)
        f2 = random.normalvariate(0, FINITE_AXIS_STDEV)

        for u, v, result in (((math.inf, f1), (math.inf, f2), 0),
                             ((math.inf, f1), (-math.inf, f2), math.radians(180)),
                             ((math.inf, f1), (f2, math.inf), math.radians(90)),
                             ((math.inf, f1), (f2, math.inf), math.radians(90)),
                             ((-math.inf, f1), (-math.inf, f2), 0),
                             ((-math.inf, f1), (f2, math.inf), math.radians(90)),
                             ((-math.inf, f1), (f2, -math.inf), math.radians(90)),
                             ((f1, math.inf), (f2, math.inf), 0),
                             ((f1, math.inf), (f2, -math.inf), math.radians(180)),
                             ((f1, -math.inf), (f2, -math.inf), 0)):
            assert abs(angle_between_r2(u, v) - result) < ABS_TOL
            assert abs(angle_between_r2(v, u) - result) < ABS_TOL


def test_both_inf() -> None:
    for x1 in INFINITIES:
        for y1 in INFINITIES:
            for x2 in INFINITIES:
                for y2 in INFINITIES:
                    assert math.isnan(angle_between_r2((x1, y1), (x2, y2)))


def test_one_axis_nan() -> None:
    N_TEST = 100
    FINITE_AXIS_STDEV = 10

    for _ in range(N_TEST):
        r = random_nonzero_length()
        theta = random_angle()
        finite_axis = random.normalvariate(0, FINITE_AXIS_STDEV)

        v = polar_to_cartesian(r, theta)

        assert math.isnan(angle_between_r2(v, (math.nan, finite_axis)))
        assert math.isnan(angle_between_r2(v, (finite_axis, math.nan)))
        assert math.isnan(angle_between_r2((math.nan, finite_axis), v))
        assert math.isnan(angle_between_r2((finite_axis, math.nan), v))


def test_both_axes_nan() -> None:
    N_TEST = 100

    for _ in range(N_TEST):
        r = random_nonzero_length()
        theta = random_angle()

        v = polar_to_cartesian(r, theta)

        assert math.isnan(angle_between_r2(v, (math.nan, math.nan)))
        assert math.isnan(angle_between_r2((math.nan, math.nan), v))


def test_both_nan() -> None:
    assert math.isnan(angle_between_r2(
        (math.nan, math.nan), (math.nan, math.nan)))
