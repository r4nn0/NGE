#include "TestPlayer.h"
TestPlayer::TestPlayer(std::string sprName):Object2D::Object2D(sprName), idle_time(0){}
TestPlayer::TestPlayer():Object2D::Object2D(), idle_time(0){}

void TestPlayer::Update(){
    int hsp=0, vsp=0;
    hsp=(keyboard_check(GLFW_KEY_RIGHT)-keyboard_check(GLFW_KEY_LEFT))*3;
    vsp=(keyboard_check(GLFW_KEY_DOWN)-keyboard_check(GLFW_KEY_UP))*3;
    if(hsp==0 && vsp==0) {idle_time++;state=0;}
    else idle_time=0;
    position+=glm::vec2(hsp,vsp);
    Object2D::Update();
    
    scale=glm::vec2(3);
    anim_speed=0.01;
    frame_index+=anim_speed;
}