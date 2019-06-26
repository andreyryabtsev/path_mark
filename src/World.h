/*
A representation of a 2D world with rectangular axis-aligned obstacles. Includes
a reference to a graph and the starting and target vertices on said graph. All
worlds span the rectangle between (0, 0) and (1, 1).
*/
#ifndef _WORLD_H_
#define _WORLD_H_
#include <vector>
#include <string>
#include "Rect.h"
#include "Drawable.h"
#include "Line.h"
class World : public Drawable {
 public:
    // Constructs an empty world
    World();
    World(const World&) = delete;
    // Reads in a world from a savefile
    World(const std::string& filepath);
    // Gets the start node id in the attached graph
    std::vector<double> getStartPosition() const;
    // Gets the target node id in the attached graph
    std::vector<double> getTargetPosition() const;
    // Checks whether a line is in collision with the obstacles in this world
    bool lineInCollision(const Line& l) const;
    // Draws the world using a Renderer
    void draw(Renderer& renderer) const override;
 private:
    // the rectangular obstacles of this world
    std::vector<Rect> obstacles_;
    // the name of the graph that this world was built for
    std::string graph_name_;
    // the start and target positions of this graph
    std::vector<double> mStartPosition, mTargetPosition;
};
#endif