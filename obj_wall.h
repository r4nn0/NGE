#ifndef OBJ_WALL_H
#define OBJ_WALL_H
#include "Objects.h"
class obj_wall : public Objects
{
public:
    obj_wall();
    obj_wall(float _x,float _y);
    obj_wall(std::string _imagePath,float _xScale, float _yScale,float _x,float _y);
    virtual ~obj_wall();
    void StepEvent();
    void DrawEvent();
    float x,y;
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

#endif // OBJ_WALL_H
