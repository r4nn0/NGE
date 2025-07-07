#ifndef TestPlayer_HEADER
#define TestPlayer_HEADER
#include "Object2D.h"

class TestPlayer : public Object2D{
public:
    using Object2D::Object2D;
    void Update () override;
}; 

#endif