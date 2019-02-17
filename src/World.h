#ifndef _WORLD_H_
#define _WORLD_H_
#include <vector>
#include "Obstacle.h"
#include "Line.h"
class World {
 public:
    bool lineInCollision(const Line& l) const;
 protected:
    std::vector<Obstacle> obstacles;
};
#endif