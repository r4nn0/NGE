#ifndef TestPlayer_HEADER
#define TestPlayer_HEADER
#include "Object2D.h"

class TestPlayer : public Object2D{
private:
    unsigned idle_time;
    unsigned state;
public:
    TestPlayer(std::string);
    TestPlayer();
    void Update();
}; 

#endif