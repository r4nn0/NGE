#ifndef TestPlayer_HEADER
#define TestPlayer_HEADER
#include "Object2D.h"

class TestPlayer : public Object2D{
public:
    TestPlayer();
    TestPlayer(const char*);
    void Update();
}; 

#endif