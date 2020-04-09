#ifndef OBJECTS_H_INCLUDED
#define OBJECTS_H_INCLUDED
#include "Graphics/Sprite.h"
#include "Assets/Input.h"
using namespace Input;
class Objects{
    public:
        Objects();
        Objects(float _x,float _y);
        Objects(std::string _imagePath,float _xScale, float _yScale,float _x,float _y);
        virtual ~Objects();
        void StepEvent();
        void DrawEvent();
        float x,y;
        int image_index, image_angle, image_speed;
        Sprite sprite_index;
        int get_id();
        friend std::ostream &operator << (std::ostream &out, Objects&);
        void DestroyEvent();
        int bbox_left,bbox_right,bbox_up,bbox_down,sprite_xoffset=0,sprite_yoffset=0;
        bool isDestroyed();
        void bg_Step();
    private:
        int id;
        bool Destroyed;
        static int instance_id;
};

#endif // OBJECTS_H_INCLUDED
