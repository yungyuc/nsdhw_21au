#include "vector.hpp"

#include <algorithm>
#include <cmath>

static double dot_r2(r2 const u, r2 const v)
{
    return u.first*v.first + u.second*v.second;
}

static r2 normalise_r2(r2 const v)
{
    if (std::isinf(v.first) && std::isfinite(v.second))
    {
        return std::make_pair(std::copysign(1, v.first), 0);
    }
    else if (std::isfinite(v.first) && std::isinf(v.second))
    {
        return std::make_pair(0, std::copysign(1, v.second));
    }
    else
    {
        const double l2_norm = std::sqrt(dot_r2(v, v));
        return std::make_pair(v.first / l2_norm, v.second / l2_norm);
    }
}

double angle_between_r2(r2 const u, r2 const v)
{
    // In rare cases, the value of dot_r2(normalise_r2(u), normalise_r2(v))
    // could go slightly over 1. So we use std::min(..., 1) to ensure that
    // std::acos doesn't return NaN.
    // Also note the argument order of std::min. This order is required to make
    // NaN propagation work.
    return std::acos(std::min(dot_r2(normalise_r2(u), normalise_r2(v)), 1.));
}
