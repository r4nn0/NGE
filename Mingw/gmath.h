#ifndef GMATH_H_INCLUDED
#define GMATH_H_INCLUDED
/// PI
#define GM_PI 3.14159265358979323846
/// PI/2
#define GM_PI_2 1.57079632679489661923
/// PI/4
#define GM_PI_4 0.785398163397448309616
/// 1/PI
#define GM_1_PI 0.318309886183790671538
/// 2/PI
#define GM_2_PI 0.636619772367581343076
/// 2/sqrt(PI)
#define GM_2_SQRTPI 1.12837916709551257390
/// sqrt(2)
#define GM_SQRT2 1.41421356237309504880
/// 1/sqrt(2)
#define GM_1_SQRT2 0.707106781186547524401
/// e
#define GM_E 2.71828182845904523536
/// log2(e)
#define GM_LOG2E 1.44269504088896340736
/// log10(e)
#define GM_LOG10E 0.434294481903251827651
/// ln(2)
#define GM_LN2 0.693147180559945309417
/// ln(10)
#define GM_LN10 2.30258509299404568402
/// NAN
#define GM_NAN __builtin_nan("")

#include <chrono>
namespace gmath{

    int sign(double);
    double abs(double);
    double min(double, double);
    double max(double, double);

    int factorial( int);
    long long irandom(long long, long long);
    double drandom(double, double);

    
    double dmod(double, double);

    double dsin(double);
    double dcos(double);
    double dtan(double);

    double asin(double);
    double acos(double);
    double atan(double);
    double atan2(double, double);

    double pow(double, double);
    double sqrt(double);
    double sqr(double);

    double ceil(double);
    double floor(double);
    double round(double);

    double radtodeg(double);
    double point_direction(double, double, double, double);
    double point_distance(double, double, double, double);
    double lengthdir_x(double, double);
    double lengthdir_y(double, double);

}
#endif // GMATH_H_INCLUDED
