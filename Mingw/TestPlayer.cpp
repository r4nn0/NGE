#include "TestPlayer.h"
TestPlayer::TestPlayer(const char* path):Object2D::Object2D(path){}
TestPlayer::TestPlayer():Object2D::Object2D(){}

void TestPlayer::Update(){
    int hsp=0, vsp=0;
    hsp=(keyboard_check(GLFW_KEY_RIGHT)-keyboard_check(GLFW_KEY_LEFT))*3;
    vsp=(keyboard_check(GLFW_KEY_DOWN)-keyboard_check(GLFW_KEY_UP))*3;
    position+=glm::vec2(hsp,vsp);
    Object2D::Update();
}