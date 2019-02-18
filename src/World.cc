#include <iostream>
#include <fstream>
#include <string>
#include "World.h"
#include "Rect.h"
#include "Line.h"
#include "Renderer.h"

static bool lineInteresectLine(const double x1, const double y1, const double x2, const double y2, const double x3, const double y3, const double x4, const double y4) {
    double uA = ((x4-x3)*(y1-y3) - (y4-y3)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
    double uB = ((x2-x1)*(y1-y3) - (y2-y1)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
    return uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1;
}

bool World::lineInCollision(const Line& l) const {
    for(Rect o : obstacles_) {
        if (lineInteresectLine(l.x1, l.y1, l.x2, l.y2, o.x1, o.y1, o.x1, o.y2)) return true;
        if (lineInteresectLine(l.x1, l.y1, l.x2, l.y2, o.x1, o.y1, o.x2, o.y1)) return true;
        if (lineInteresectLine(l.x1, l.y1, l.x2, l.y2, o.x2, o.y2, o.x1, o.y2)) return true;
        if (lineInteresectLine(l.x1, l.y1, l.x2, l.y2, o.x2, o.y2, o.x2, o.y1)) return true;
    }
    return false;
}

World::World() {
    obstacles_ = std::vector<Rect>();
}

World::World(const std::string& filepath) {
    obstacles_ = std::vector<Rect>();
    std::ifstream worldfile;
    worldfile.open(filepath);
    if (!worldfile.is_open()) throw;
    int numObstacles;
    worldfile >> numObstacles;
    for(int i = 0; i < numObstacles; i++) {
        double x1, y1, x2, y2;
        worldfile >> x1 >> y1 >> x2 >> y2;
        Rect o(x1, y1, x2, y2);
        obstacles_.push_back(o);
    }
    worldfile.close();
}

void World::draw(Renderer& renderer) {
    renderer.clear();
    
}
