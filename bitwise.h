#ifndef BITWISE_H_INCLUDED
#define BITWISE_H_INCLUDED
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include "gmath.h"
template<typename LHS, typename OP>
struct LHSlt {
    LHS lhs_;
};
enum { _ror };
template<typename LHS>
LHSlt<LHS, decltype(_ror)> operator<(const LHS& lhs, decltype(_ror))
{
    return { lhs };
}
/// Rotate Bits Right
int operator>(LHSlt<int, decltype(_ror)> lhsof, int rhs)
{
    int& lhs = lhsof.lhs_;
    int i=2,j=2,bit=0;
    while(i<lhs){
        i*=2;
        bit++;
    }
    std::vector<int> bits(bit+1);
    std::fill(bits.begin(),bits.end(),0);
    while(bit>0 || lhs<0){
        while(lhs<i/j){
            j*=2;
            bit--;
        }
        bits[bit]=1;
        lhs-=gmath::pow(2,bit);
    }
    std::rotate(bits.begin(),bits.begin()+rhs,bits.end());
    int res=0;
    for(size_t l=0;l<bits.size();l++){
        if(bits[l])
            res+=gmath::pow(2,l);
    }
    return (res);
}
/// Rotate Bits Left
enum { _rol };
template<typename LHS>
LHSlt<LHS, decltype(_rol)> operator<(const LHS& lhs, decltype(_rol))
{
    return { lhs };
}
int operator>(LHSlt<int, decltype(_rol)> lhsof, int rhs)
{
    int& lhs = lhsof.lhs_;
    int i=2,j=2,bit=0;
    while(i<lhs){
        i*=2;
        bit++;
    }
    std::vector<int> bits(bit+1);
    std::fill(bits.begin(),bits.end(),0);
    while(bit>0 || lhs<0){
        while(lhs<i/j){
            j*=2;
            bit--;
        }
        bits[bit]=1;
        lhs-=gmath::pow(2,bit);
    }
    std::rotate(bits.begin(),bits.begin()+bits.size()-rhs,bits.end());
    int res=0;
    for(size_t l=0;l<bits.size();l++){
        if(bits[l])
            res+=gmath::pow(2,l);
    }
    return (res);
}

#define AND &
#define OR |
#define XOR ^
#define SHR >>
#define SHL <<
#define ROR <_ror>
#define ROL <_rol>

namespace bitwise{
    const char* dec_to_hex(int);
    int hex_to_dec(const char*);
    int NOT(int);
}


#endif // BITWISE_H_INCLUDED
