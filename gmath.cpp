#include "gmath.h"
int gmath::sign(double n){
    return (n<0) ? -1:1;
}
int gmath::irandom(int maxValue){
    std::chrono::time_point<std::chrono::system_clock> ms = std::chrono::system_clock::now();
    int retVal;
    retVal=(ms.time_since_epoch().count()%(maxValue*1000+1))/1000;
    usleep(1);
    if((ms.time_since_epoch().count()%(maxValue*1000+1)/1000)%2)
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
    if (Value<0)
        return Value*-1;
    return Value;
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

double gmath::pow(double base, double exp){
    double res=base;
    int i;
    if(exp>0){i=1;while(i<exp){res*=base;i++;}}
    else{i=0;while(i>=exp){res/=base;i--;}}
    return res;
}
double gmath::sqrt(double n){
    double temp=0,sqt=n/2;
    if(n<0) return NAN;
    while(sqt!=temp){
        temp=sqt;
        sqt=(n/temp+temp)/2;
    }
    return sqt;
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