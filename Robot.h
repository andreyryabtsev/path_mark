#include "World.h"

#ifndef _ROBOT_H_
class Robot {
 public:
    void assignWorld(const World &world);
 protected:
    World world_;
};
#endif