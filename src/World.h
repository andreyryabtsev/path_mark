#ifndef _WORLD_H_
#define _WORLD_H_
#include <vector>
#include <string>
#include "Rect.h"
#include "Drawable.h"
#include "Line.h"
class World : Drawable {
 public:
    World();
    World(const std::string& filepath);
    bool lineInCollision(const Line& l) const;
    void draw(Renderer& renderer) override;
 private:
    std::vector<Rect> obstacles_;
};
#endif