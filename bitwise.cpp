#include "bitwise.h"
/// Convert From base 10 to base 16, returns string
const char* bitwise::dec_to_hex(int dec){
    std::string hex,temp;
    char hi,lo;
    const char* hexnums="0123456789ABCDEF";
    int b;
    if (dec!=0) hex="";
    else hex="00";

    while(dec!=0){
        b=dec&255;
        hi=hexnums[b/16];
        lo=hexnums[b%16];
        temp=hex;
        hex="00";
        hex[0]=hi;
        hex[1]=lo;
        hex.append(temp);
        dec>>=8;
    }
    return hex.c_str();
}
/// Convert From base 16 to base 10, takes string
int bitwise::hex_to_dec(const char* hex){
    int base = 1;
    int dec = 0;
    for (int i=strlen(hex)-1; i>=0; i--){
        if (hex[i]>='0' && hex[i]<='9'){
            dec += (hex[i] - 48)*base;
            base *= 16;
        }
        else if (hex[i]>='A' && hex[i]<='F'){
            dec += (hex[i] - 55)*base;
            base *= 16;
        }
    }
    return dec;
}
/// Inverts Bits in a number
int bitwise::NOT(int x){
    int i=2;
    while(i<x)i*=2;
    i*=2-1;
    return x XOR i;
}
