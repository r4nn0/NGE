#include "ngestd.h"
std::vector<std::reference_wrapper<Sprite>> SpritesToRender;
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
void ngestd::DrawPoint(float x, float y){
    glBegin(GL_POINTS);
    glVertex2f(x,y);
    glEnd();
}
void ngestd::DrawRectangle(float x1, float y1, float x2, float y2, bool outline){
    glBegin(GL_QUADS);
    if(outline)
    glBegin(GL_LINE_LOOP);
    glVertex2f(x1,y1);
    glVertex2f(x2,y1);
    glVertex2f(x2,y2);
    glVertex2f(x1,y2);
    glEnd();
}
void ngestd::DrawSetColor(glm::vec3 col){
    glColor3f(col.x,col.y,col.z);
}
glm::vec3 ngestd::DrawGetColor(){
    float col[3];
    glGetFloatv(GL_CURRENT_COLOR,col);
    return glm::make_vec3(col);
}