#include "TestPlayer.h"
void TestPlayer::Update(){
    Object2D::Update();
    int hsp=0, vsp=0;
    hsp=(keyboard_check(GLFW_KEY_RIGHT)-keyboard_check(GLFW_KEY_LEFT));
    vsp=(keyboard_check(GLFW_KEY_DOWN)-keyboard_check(GLFW_KEY_UP));
    //if(hsp==0 && vsp==0) {idle_time++;state=0;}
    //else idle_time=0;
    position+=glm::vec3(hsp,vsp,0);
    
    
    scale=glm::vec2(3);
    anim_speed=0.01;
    frame_index+=anim_speed;
}