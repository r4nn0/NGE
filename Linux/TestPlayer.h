#ifndef TestPlayer_HEADER
#define TestPlayer_HEADER
#include "Object2D.h"

class TestPlayer : public Object2D{
public:
    TestPlayer(const char*);
    TestPlayer();
    void Update();
}; 

#endif