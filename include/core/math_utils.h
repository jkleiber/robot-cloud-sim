
#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include "core/logging.h"

template <typename T> bool Clamp(T *val, const T &min_val, const T &max_val)
{
    VERIFY(val != nullptr);

    // Min value must be less than maximum value.
    if (min_val > max_val)
    {
        return false;
    }

    // Ensure the value is not below the minimum or above the maximum.
    if (*val < min_val)
    {
        *val = min_val;
    }
    else if (*val > max_val)
    {
        *val = max_val;
    }

    return true;
}

bool DegToRad(double *val);
bool RadToDeg(double *val);
