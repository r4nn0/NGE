#include "gmath.h"
/**
 * @brief Return the sign value of a number
 * 
 * @param n number in double
 * @return int returns -1 if the number is negative, 0 if 0 and 1 if more than 0
 */
int gmath::sign(double n){
    if(n==0) return 0;
    return (n<0) ? -1:1;
}
/**
 * @brief Determines which if the parameters has the lowest value
 * 
 * @param a first value
 * @param b second value
 * @return double lowest value
 */
double gmath::min(double a, double b){
    return (a<b) ? a:b;
}
/**
 * @brief Determines which if the parameters has the highest value
 * 
 * @param a first value
 * @param b second value
 * @return double highest value
 */
double gmath::max(double a, double b){
    return (a>b) ? a:b;
}
/**
 * @brief Generates a random number in range (Including minimum and maximum number in the range)
 * 
 * @param r1 first value in the range
 * @param r2 last value in the range 
 * @return long long return the generated random number
 */
long long gmath::irandom(long long r1, long long r2){
    static unsigned long long seed=std::chrono::system_clock::now().time_since_epoch().count();
    long long retVal;
    seed ^= seed<<26;
    seed ^= seed>>34;
    seed ^= seed<<10;
    long long range = abs(r1-r2)+1;
    retVal= (seed%range) + min(r1,r2);
    return retVal;
}
/**
 * @brief Generates a random number in range (Including minimum and maximum number in the range) with decimals
 * 
 * @param r1 first value in the range
 * @param r2 last value in the range 
 * @return double return the generated random number
 */
double gmath::drandom(double a, double b){
    return gmath::min(gmath::max(a,b),
                      (double)gmath::irandom(a,b)+
                      (double)gmath::irandom(0,gmath::pow(10,15))/gmath::pow(10,15));
}
/**
 * @brief Calculates the remainder of a double divided by another
 * 
 * @param val Value to divide
 * @param div divisor
 * @return double remainder of the division (val%div)
 */
double gmath::dmod(double val, double div){
    return __builtin_fmod(val, div);
}
/**
 * @brief Calculates factorial of the number passed in as parameter
 * 
 * @param n number to calculate factorial for
 * @return int return result of the factorial
 */
int gmath::factorial(int n){
    if(n > 1)
        return n * factorial(n - 1);
    else
        return 1;
}
/**
 * @brief Returns the absolute value of a number
 * 
 * @param Value number in double
 * @return double returns absolute value in double
 */
double gmath::abs(double Value){
    return (Value<0) ? Value*-1: Value;
}
/**
 * @brief Calculates the sinus value from degrees
 * 
 * @param angle angle in degrees
 * @return double returns the sinus value in double
 */
double gmath::dsin(double angle){
    int inv=1;
    if(angle<0) inv=-1;
    angle=gmath::abs(angle);
    while(angle>=180){angle-=180;inv*=-1;}
    angle*=GM_PI/180;
    return __builtin_sin(angle)*inv;
}
/**
 * @brief Calculates the cosinus value from degrees
 * 
 * @param angle angle in degrees
 * @return double returns the cosinus value in double
 */
double gmath::dcos(double angle){
    int inv=1;
    if(angle<0) inv=-1;
    angle=gmath::abs(angle);
    while(angle>=180){angle-=180;inv*=-1;}
    angle*=GM_PI/180;
    return __builtin_cos(angle)*inv;
}
/**
 * @brief Calculates the tangent value from degrees
 * 
 * @param angle angle in degrees
 * @return double returns the tangent value in double
 */
double gmath::dtan(double angle){
    int inv=1;
    if(angle<0) inv=-1;
    angle=gmath::abs(angle);
    while(angle>=180){angle-=180;inv*=-1;}
    angle*=GM_PI/180;
    return __builtin_tan(angle)*inv;
}
/**
 * @brief Calculate the angle from sinus value
 * 
 * @param x value in sinus
 * @return double angle in degrees
 */
double gmath::asin(double x){
    return __builtin_asin(x)*180/GM_PI;
}
/**
 * @brief Calculate the angle from cosinus value
 * 
 * @param x value in cosinus
 * @return double angle in degrees
 */
double gmath::acos(double x){
    return __builtin_acos(x)*180/GM_PI;
}
/**
 * @brief Calculate the angle from tangent value
 * 
 * @param x value in tangent
 * @return double angle in degrees
 */
double gmath::atan(double x){
    return __builtin_atan(x)*180/GM_PI;
}
/**
 * @brief Calculate the angle from cosinus and sinus values
 * 
 * @param x value in cosinus
 * @param y value in sinus
 * @return double angle in degrees
 */
double gmath::atan2(double x, double y){
    return __builtin_atan2l(x,y);
}
/**
 * @brief Calculates the power of a number
 * 
 * @param base The number to calculate power for
 * @param exp exponent in double
 * @return double return result in double
 */
double gmath::pow(double base, double exp){
    return __builtin_pow(base, exp);
}
/**
 * @brief Calculate the square root of a number
 * 
 * @param n number to calculate the square root of
 * @return double square root in double
 */
double gmath::sqrt(double n){
    return __builtin_sqrt(n);
}
/**
 * @brief Returns the square value of a number
 * 
 * @param n value in double
 * @return double result of the square
 */
double gmath::sqr(double n){
    return n*n;
}
/**
 * @brief Rounds a floating value upwards
 * 
 * @param n value in double
 * @return double returns the value rounded up
 */
double gmath::ceil(double n){
    if(n>0){
        n*=10;
        int temp=n;
        temp%=10;
        if(temp>0) return floor(n/10)+1;
        return floor(n/10);
    }
    return floor(n);
}
/**
 * @brief Rounds a floating value downwards
 * 
 * @param n value in double
 * @return double returns the value rounded down
 */
double gmath::floor(double n){
    return (double)((long long int)n);
}
/**
 * @brief Rounds a value normally
 * 
 * @param n value to round
 * @return double returns rounded value
 */
double gmath::round(double n){
    int temp;
    if(n>0){
        n*=10;
        temp=n;
        temp%=10;
        if(temp>=5) return floor(n/10)+1;
        return floor(n/10);
    }
    n*=10;
    temp=n;
    temp%=10;
    if(temp<=-5) return floor(n/10)-1;
    return floor(n/10);
}
/**
 * @brief Convert radians to degrees
 * 
 * @param radian value in radians
 * @return double value in degrees
 */
double gmath::radtodeg(double radian){
    return radian*180/GM_PI;
}
/**
 * @brief Returns the angle between two points
 * 
 * @param x1 x coords of the first point
 * @param y1 y coords of the first point
 * @param x2 x coords of the second point
 * @param y2 y coords of the second point
 * @return double returns angle in degrees 
 */
double gmath::point_direction(double x1, double y1, double x2, double y2){
    return 180-gmath::radtodeg(gmath::atan2(y1-y2,x1-x2));
}
/**
 * @brief Calculates distance between two points
 * 
 * @param x1 x coords of the first point
 * @param y1 y coords of the first point
 * @param x2 x coords of the second point
 * @param y2 y coords of the second point
 * @return double returns distance in double
 */
double gmath::point_distance(double x1, double y1, double x2, double y2){
    return sqrt(gmath::abs(y2-y1)*gmath::abs(y2-y1)+gmath::abs(x2-x1)*gmath::abs(x2-x1));
}
/**
 * @brief Calculates the length along the x-axis 
 * 
 * @param dist distance to calculate
 * @param angle angle of the direction
 * @return double the distance along x-axis
 */
double gmath::lengthdir_x(double dist, double angle){
    return dist*gmath::dcos(angle);
}
/**
 * @brief Calculates the length along the y-axis 
 * 
 * @param dist distance to calculate
 * @param angle angle of the direction
 * @return double the distance along y-axis
 */
double gmath::lengthdir_y(double dist, double angle){
    return dist * -gmath::dsin(angle);
}
