#ifndef _MESHLIB_POINT_H_
#define _MESHLIB_POINT_H_

#include <assert.h>
#include <cmath>


namespace MeshLib {

#if !defined(PI)
    const double PI = 3.1415926536;
#endif

#if !defined(sqr)
    static double sqr(double x)
    {
        return x*x;
    }
#endif

class CPoint {
    public:
        inline CPoint& operator=(const CPoint& rhs)
        {
            if (this == &rhs) return *this;
            v[0] = rhs.v[0];
            v[1] = rhs.v[1];
            v[2] = rhs.v[2];
            return *this;
        };

        CPoint(double x, double y, double z) {
            v[0] = x;
            v[1] = y;
            v[2] = z;
        };
        CPoint() {
            v[0] = v[1] = v[2] = 0;
        };
        ~CPoint() {};

        double& operator[](int i)        {
            assert(0<=i && i<3);
            return v[i];
        };
        double   operator()(int i) const {
            assert(0<=i && i<3);
            return v[i];
        };
        double   operator[](int i) const {
            assert(0<=i && i<3);
            return v[i];
        };
        double norm() const {
            return sqrt(fabs(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]));
        };

        CPoint&   operator += (const CPoint& p) {
            v[0] += p(0);
            v[1] += p(1);
            v[2] += p(2);
            return *this;
        };
        CPoint&   operator -= (const CPoint& p)  {
            v[0] -= p(0);
            v[1] -= p(1);
            v[2] -= p(2);
            return *this;
        };
        CPoint&   operator *= (const double  s) {
            v[0] *= s   ;
            v[1] *=    s;
            v[2] *=    s;
            return *this;
        };
        CPoint&   operator /= (const double  s) {
            v[0] /= s   ;
            v[1] /=    s;
            v[2] /=    s;
            return *this;
        };

        CPoint   operator+(const CPoint& p) const
        {
            CPoint r(v[0] + p[0], v[1] + p[1], v[2] + p[2]);
            return r;
        };
        CPoint   operator-(const CPoint& p) const
        {
            CPoint r(v[0] - p[0], v[1] - p[1], v[2] - p[2]);
            return r;
        };
        CPoint   operator*(const double s) const
        {
            CPoint r(v[0] * s, v[1] * s, v[2] * s);
            return r;
        };
        CPoint   operator/(const double s) const
        {
            CPoint r(v[0] / s, v[1] / s, v[2] / s);
            return r;
        };

		//dot product
		double   operator*(const CPoint& p) const
        {
            return v[0] * p[0] + v[1] * p[1] + v[2] * p[2];
        };

		//cross product
        CPoint operator^(const CPoint& p2) const
        {
            CPoint r(v[1] * p2[2] - v[2] * p2[1],
                     v[2] * p2[0] - v[0] * p2[2],
                     v[0] * p2[1] - v[1] * p2[0]);
            return r;
        };

		//reverse
        CPoint operator-() const
        {
            CPoint p(-v[0],-v[1],-v[2]);
            return p;
        };
    public:
        double v[3];
};	


bool operator==(const CPoint& pnt1, const CPoint& pnt2);
bool operator<(const CPoint& pnt1, const CPoint& pnt2);
double distance(const CPoint& point1, const CPoint& point2);

double computer_angle_abc(CPoint &corner, CPoint &side1, CPoint &side2);
}//name space MeshLib

#endif //_MESHLIB_POINT_H_ defiined