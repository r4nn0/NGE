#include "obj_player.h"
int obj_player::instance_id=100000;
obj_player::obj_player(){id=instance_id;instance_id++;Destroyed=false;}
obj_player::obj_player(float _x,float _y){x=_x;y=_y;id=instance_id;instance_id++;Destroyed=false;}
obj_player::obj_player(std::string _imagePath,float _xScale, float _yScale,float _x,float _y){
        x=_x;
        y=_y;
        id=instance_id;instance_id++;
        sprite_index=Sprite(_imagePath);
        sprite_index.xscale=_xScale;
        sprite_index.yscale=_yScale;
        Destroyed=false;
}
obj_player::~obj_player(){
    if (!Destroyed){
        instance_id--;
        Destroyed=true;
    }
}
void obj_player::StepEvent(){
    if(!Destroyed){
        bg_Step();
        int key_down=(int)keyboard_check(GLFW_KEY_DOWN);
        int key_up=(int)keyboard_check(GLFW_KEY_UP);
        int key_left=(int)keyboard_check(GLFW_KEY_LEFT);
        int key_right=(int)keyboard_check(GLFW_KEY_RIGHT);
        hsp=(key_right-key_left)*3.7;
        vsp=(key_down-key_up)*3.7;
        x+=hsp;
        y+=vsp;
    }
}
void obj_player::bg_Step(){
    bbox_up=sprite_index.sprite_get_y();
    bbox_right=sprite_index.sprite_get_x()+sprite_index.sprite_get_width()*sprite_index.xscale;
    bbox_left=sprite_index.sprite_get_x();
    bbox_down=sprite_index.sprite_get_y()+sprite_index.sprite_get_height()*sprite_index.yscale;
}
void obj_player::DrawEvent(){
    if(!Destroyed){
        sprite_index.sprite_set_pos(x-sprite_xoffset,y-sprite_yoffset);
        sprite_index.Render();
    }
}
void obj_player::DestroyEvent(){
    delete this;
}
bool obj_player::isDestroyed(){
    return Destroyed;
}
int obj_player::get_id(){
    if(!Destroyed){
        return id;
    }
    else return -1;
}
