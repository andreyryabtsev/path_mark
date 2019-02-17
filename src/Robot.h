#include "World.h"
#include <vector>

#ifndef _ROBOT_H_
#define _ROBOT_H_
class Robot : Drawable{
 public:
    virtual bool inCollision(const World&) = 0;
 protected:
};
#endif