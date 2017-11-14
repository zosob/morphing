#include "Point.h"
#include <math.h>

namespace MeshLib
{

#if !defined(__sqr)
    template <typename T>
    inline const T __sqr(const T& a)
    {
        return a*a;
    }
#endif

const double PRECISION = 0.0000000001;

bool operator==(const CPoint& pnt1, const CPoint& pnt2)
{
    return (pnt1.v[0] == pnt2.v[0]
        && pnt1.v[1] == pnt2.v[1]
        && pnt1.v[2] == pnt2.v[2]);
}


bool operator<(const CPoint& pnt1, const CPoint& pnt2)
{
    double a = pnt1.v[0] - pnt2.v[0];
    if (a < -0.0000000001)
        return true;
    else if (a > 0.0000000001)
        return false;

    a = pnt1.v[1] - pnt2.v[1];
    if (a < -0.0000000001)
        return true;
    else if (a > 0.0000000001)
        return false;

    a = pnt1.v[2] - pnt2.v[2];
    if (a < -0.0000000001)
        return true;

    return false;

}

double distance(const CPoint& point1, const CPoint& point2)
{
    double tmp = __sqr(point1.v[0] - point2.v[0]) + __sqr(point1.v[1] - point2.v[1]) + __sqr(point1.v[2] - point2.v[2]);
    if (fabs(tmp) < PRECISION)
    {
        return 0;
    }
    else
    {
        return sqrt(tmp);
    }
}

double computer_angle_abc(CPoint &corner, CPoint &side1, CPoint &side2)
{
    double a = fabs(distance(corner, side1));
    double b = fabs(distance(corner, side2));
    double c = fabs(distance(side1, side2));

    double cos_alpha = (a*a + b*b - c*c) / (2 * a*b);
    return acos(cos_alpha);
}

}