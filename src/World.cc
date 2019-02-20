#include <iostream>
#include <fstream>
#include <string>
#include "World.h"
#include "Rect.h"
#include "Line.h"
#include "Renderer.h"

// private static helper that checks if two line segments intersect each other
static bool lineInteresectLine(const double x1, const double y1, const double x2,
    const double y2, const double x3, const double y3, const double x4, const double y4) {
    // check if two lines intersect each other using a formula
    // shamelessly stolen from online
    double uA = ((x4-x3)*(y1-y3) - (y4-y3)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
    double uB = ((x2-x1)*(y1-y3) - (y2-y1)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
    return uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1;
}

bool World::lineInCollision(const Line& l) const {
    // A line is in collision with this world if, for at least one of the obstacles,
    // it interesects an edge line of the obstacle
    for(Rect o : obstacles_) {
        if (lineInteresectLine(l.x1, l.y1, l.x2, l.y2, o.x1, o.y1, o.x1, o.y2)) return true;
        if (lineInteresectLine(l.x1, l.y1, l.x2, l.y2, o.x1, o.y1, o.x2, o.y1)) return true;
        if (lineInteresectLine(l.x1, l.y1, l.x2, l.y2, o.x2, o.y2, o.x1, o.y2)) return true;
        if (lineInteresectLine(l.x1, l.y1, l.x2, l.y2, o.x2, o.y2, o.x2, o.y1)) return true;
    }
    return false;
}

int World::getStartId() { return start_node_; }
int World::getTargetID() { return target_node_; }

World::World() {
    obstacles_ = std::vector<Rect>();
}

World::World(const std::string& filepath) {
    obstacles_ = std::vector<Rect>();
    std::ifstream worldfile;
    worldfile.open(filepath);
    if (!worldfile.is_open()) throw;
    worldfile >> graph_name_ >> start_node_ >> target_node_;
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
    renderer.setColor(255, 255, 255);
    // clear everything first
    renderer.clear();
    // faint gridlines
    renderer.setColor(180, 180, 180);
    for(int i = 1; i < 10; i++) {
        renderer.drawLine(Line(i / 10.0, 0, i / 10.0, 1));
        renderer.drawLine(Line(0, i / 10.0, 1, i / 10.0));
    }
    // solid black obstacles
    renderer.setColor(0, 0, 0);
    for(Rect o : obstacles_) {
        renderer.fillRect(o);
    }
}
