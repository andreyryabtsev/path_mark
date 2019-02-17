#ifndef _WORLD_H_
#define _WORLD_H_
#include <vector>
#include "Obstacle.h"
#include "Drawable.h"
#include "Line.h"
class World : Drawable {
 public:
    bool lineInCollision(const Line& l) const;
    void draw(Renderer& renderer) override;
 protected:
    std::vector<Obstacle> obstacles;
};
#endif