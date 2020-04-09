#ifndef OBJ_PLAYER_INCLUDED
#define OBJ_PLAYER_INCLUDED

#include "Objects.h"
class obj_player : public Objects
{
    public:
        obj_player();
        obj_player(float _x,float _y);
        obj_player(std::string _imagePath,float _xScale, float _yScale,float _x,float _y);
        virtual ~obj_player();
        void StepEvent();
        void DrawEvent();
        float x,y,hsp,vsp,grav;
        int image_index, image_angle, image_speed;
        Sprite sprite_index;
        int get_id();
        void DestroyEvent();
        int bbox_left,bbox_right,bbox_up,bbox_down,sprite_xoffset=0,sprite_yoffset=0;
        bool isDestroyed();
        void bg_Step();
    private:
        int id;
        bool Destroyed;
        static int instance_id;

};

#endif // OBJ_PLAYER_H
