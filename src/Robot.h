#include "World.h"
#include <vector>

#ifndef _ROBOT_H_
#define _ROBOT_H_
class Robot {
 public:
    virtual bool inCollision(const std::vector<double>&, const World&);
 protected:
};
#endif