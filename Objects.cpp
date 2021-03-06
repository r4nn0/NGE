#include "Objects.h"
Objects::Objects(){}
Objects::Objects(float _x,float _y){x=_x;y=_y;}
Objects::Objects(std::string _imagePath,float _xScale, float _yScale,float _x,float _y){
        x=_x;
        y=_y;
        sprite_index=Sprite(_imagePath);
        sprite_index.xscale=_xScale;
        sprite_index.yscale=_yScale;
}
Objects::~Objects(){
    if(!Destroyed){
        Destroyed=true;
    }
}
void Objects::StepEvent(){
    bg_Step();
}
void Objects::bg_Step(){
    bbox_up=sprite_index.sprite_get_y()-sprite_yoffset;
    bbox_right=sprite_index.sprite_get_x()+sprite_index.sprite_get_width()*sprite_index.xscale-sprite_xoffset;
    bbox_left=sprite_index.sprite_get_x()+sprite_xoffset;
    bbox_down=sprite_index.sprite_get_y()+sprite_index.sprite_get_height()*sprite_index.yscale+sprite_yoffset;
}
void Objects::DrawEvent(){
    if(!Destroyed){
        sprite_index.sprite_set_pos(x-sprite_index.sprite_get_width()/2,y-sprite_index.sprite_get_height()/2);
        sprite_index.Render();
    }
}
void Objects::DestroyEvent(){
    delete this;
}
bool Objects::isDestroyed(){
    return Destroyed;
}
