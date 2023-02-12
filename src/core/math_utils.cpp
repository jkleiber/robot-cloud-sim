
#include "core/math_utils.h"

bool DegToRad(double *val)
{
    VERIFY(val != nullptr);

    *val = *val * M_PI / 180.0;
    return true;
}

bool RadToDeg(double *val)
{
    VERIFY(val != nullptr);

    *val = *val * 180.0 / M_PI;
    return true;
}