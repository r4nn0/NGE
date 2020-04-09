#include "obj_wall.h"
int obj_wall::instance_id=100000;
obj_wall::obj_wall(){id=instance_id;instance_id++;Destroyed=false;}
obj_wall::obj_wall(float _x,float _y){x=_x;y=_y;id=instance_id;instance_id++;Destroyed=false;}

obj_wall::obj_wall(std::string _imagePath,float _xScale, float _yScale,float _x,float _y){
        x=_x;
        y=_y;
        id=instance_id;instance_id++;
        sprite_index=Sprite(_imagePath);
        sprite_index.xscale=_xScale;
        sprite_index.yscale=_yScale;
        Destroyed=false;
}
obj_wall::~obj_wall(){
    if(!Destroyed){
        instance_id--;
        Destroyed=true;
    }
}
void obj_wall::StepEvent(){
    bg_Step();
}
void obj_wall::bg_Step(){
    bbox_up=sprite_index.sprite_get_y();
    bbox_right=sprite_index.sprite_get_x()+sprite_index.sprite_get_width()*sprite_index.xscale;
    bbox_left=sprite_index.sprite_get_x();
    bbox_down=sprite_index.sprite_get_y()+sprite_index.sprite_get_height()*sprite_index.yscale;
}
void obj_wall::DrawEvent(){
    if(!Destroyed){
        sprite_index.sprite_set_pos(x-sprite_xoffset,y-sprite_yoffset);
        sprite_index.Render();
    }
}
void obj_wall::DestroyEvent(){
    delete this;
}
bool obj_wall::isDestroyed(){
    return Destroyed;
}
int obj_wall::get_id(){
    if(!Destroyed){
        return id;
    }
    else return -1;
}

