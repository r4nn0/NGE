#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "gmath.h"
#include "Objects.h"
#include <vector>

namespace CollisionSystem
{
    auto collision_circle(float x, float y,float rad, auto obj){
        for(int i=0;i<=360;i+=1){
            int _x=(float)x+gmath::dcos(i)*rad;
            int _y=(float)y+gmath::dsin(i)*rad;
            if (_x>=obj.bbox_left && _y>=obj.bbox_up && _x<=obj.bbox_right && _y<=obj.bbox_down){
                return obj;
            }
        }
        return obj;
    }
    bool collision_line_line(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4){
        float uA = ((x4-x3)*(y1-y3) - (y4-y3)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
        float uB = ((x2-x1)*(y1-y3) - (y2-y1)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
        if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1)
            return true;
        return false;
    }
    int collision_line(float x1,float y1,float x2,float y2, auto obj){
        float bbox_left= obj.bbox_left;
        float bbox_right= obj.bbox_right;
        float bbox_up= obj.bbox_up;
        float bbox_down= obj.bbox_down;

        bool left =   collision_line_line(x1,y1,x2,y2, bbox_left, bbox_up, bbox_left,bbox_down);
        bool right =  collision_line_line(x1,y1,x2,y2, bbox_right,bbox_up, bbox_right,bbox_down);
        bool top =    collision_line_line(x1,y1,x2,y2, bbox_left,bbox_up, bbox_right,bbox_up);
        bool bottom = collision_line_line(x1,y1,x2,y2, bbox_left,bbox_down, bbox_right,bbox_down);
        std::cout << obj.bbox_left << " " << right << " " << top << " " << bottom << " " << std::endl;
        if (left || right || top || bottom) {
            return obj;
        }
        return 0;
    }
}
#endif // COLLISIONSYSTEM_H
