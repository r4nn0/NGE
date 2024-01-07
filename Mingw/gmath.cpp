#include "gmath.h"
int gmath::sign(double n){
    return (n<0) ? -1:1;
}
int gmath::irandom(int maxValue){
    std::chrono::time_point<std::chrono::system_clock> ms = std::chrono::system_clock::now();
    static int seed=ms.time_since_epoch().count();
    int retVal;
    retVal=(seed%(maxValue*1000+1))/1000;
    seed*=seed;
    if(retVal%2)
        return -retVal;
    return retVal;
}
int gmath::factorial(int n){
    if(n > 1)
        return n * factorial(n - 1);
    else
        return 1;
}

double gmath::abs(double Value){
    return (Value<0) ? Value*-1: Value;
}

double gmath::dsin(double angle){
    int inv=1;
    if(angle<0) inv=-1;
    angle=gmath::abs(angle);
    while(angle>=180){angle-=180;inv*=-1;}
    angle*=GM_PI/180;
    return __builtin_sin(angle)*inv;
}
double gmath::dcos(double angle){
    int inv=1;
    if(angle<0) inv=-1;
    angle=gmath::abs(angle);
    while(angle>=180){angle-=180;inv*=-1;}
    angle*=GM_PI/180;
    return __builtin_cos(angle)*inv;
}
double gmath::dtan(double angle){
    int inv=1;
    if(angle<0) inv=-1;
    angle=gmath::abs(angle);
    while(angle>=180){angle-=180;inv*=-1;}
    angle*=GM_PI/180;
    return __builtin_tan(angle)*inv;
}

double gmath::asin(double x){
    return __builtin_asin(x)*180/GM_PI;
}
double gmath::acos(double x){
    return __builtin_acos(x)*180/GM_PI;
}
double gmath::atan(double x){
    return __builtin_atan(x)*180/GM_PI;
}
double gmath::atan2(double x, double y){
    return __builtin_atan2l(x,y);
}
double gmath::pow(double base, double exp){
    double res=base;
    int i=0;
    if(exp>0){i=1;while(i<exp){res*=base;i++;}}
    else{while(i>=exp){res/=base;i--;}}
    return res;
}
double gmath::sqrt(double n){
    double temp=0,sqt=n/2;
    if(n<0) return GM_NAN;
    while(sqt!=temp){
        temp=sqt;
        sqt=(n/temp+temp)/2;
    }
    return sqt;
}
double gmath::sqr(double n){
    return n*n;
}

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
double gmath::floor(double n){
    return (double)((long long int)n);
}
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
double gmath::radtodeg(double radian){
    return radian*180/GM_PI;
}
double gmath::point_direction(double x1, double y1, double x2, double y2){
    return 180-gmath::radtodeg(gmath::atan2(y1-y2,x1-x2));
}
double gmath::point_distance(double x1, double y1, double x2, double y2){
    return sqrt(gmath::abs(y2-y1)*gmath::abs(y2-y1)+gmath::abs(x2-x1)*gmath::abs(x2-x1));
}
double gmath::lengthdir_x(double dist, double angle){
    return dist*gmath::dcos(angle);
}
double gmath::lengthdir_y(double dist, double angle){
    return dist * -gmath::dsin(angle);
}
