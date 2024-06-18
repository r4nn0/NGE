#include "ngestd.h"
std::vector<Sprite> SpritesToRender;
std::wstring ar_fix(std::wstring str){
    for(size_t i = 0; i<str.size();i++){
        
        size_t pos = ar_has_mi.find(str[i]);
        if(pos!=std::wstring::npos) {
            str[i]=has_mi[pos];
            continue;
        }
        pos = ar_has_no_mi.find(str[i]);
        if(pos!=std::wstring::npos) str[i]=has_no_mi[pos];
    }
    std::wstring out = str;
    
    bool cp_has_mi=false;
    for(size_t i = 0; i<str.size();i++){
        wchar_t c = str[i];
        wchar_t cn = str[(i<str.size()) ? i+1:i];
        bool c_has_mi = (has_mi.find(c)!=std::wstring::npos) ? true:false;
        bool cn_is_alphabet = (ar_alphabet.find(cn)!=std::wstring::npos) ? true:false;
        if(c_has_mi){
            if(!cp_has_mi && cn_is_alphabet) c+=2;
            else if(cp_has_mi && cn_is_alphabet) c+=3;
            else if(cp_has_mi) c+=1;
            cp_has_mi=true;
            out[i]=c;
            continue;
        }
        bool c_has_no_mi = (has_no_mi.find(c)!=std::wstring::npos) ? true:false;
        if(cp_has_mi && c_has_no_mi) c++;
        cp_has_mi=false;
        out[i]=c;
    }
    std::wstring temp=out;
    for(size_t j=0;j<out.size();j++){
        out[j]=temp[out.size()-1-j];
    }
    return out;
}