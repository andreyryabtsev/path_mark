/*
A template derived class of Robot. Represents a robot consisting of N arms that
start at (0.5, 0.5) and extend links of length 0.15 from there.
*/

#ifndef _N_LINK_ARM_H_
#define _N_LINK_ARM_H_

#include <assert.h>
#include <math.h>
#include "Robot.h"
#include "Line.h"

#define LINK_LENGTH 0.15
#define ROOT_X 0.5
#define ROOT_Y 0.5

template <int N>
class NLinkArm : public Robot {
 public:
    // Default constructor is an arm extended right.
    NLinkArm() {
        for(int i = 0; i < N; i++) mState.push_back(0.0);
    }

    // Constructor that accepts an initial state
    NLinkArm(std::vector<double> state) {
        assert(state.size() == N);
        mState = state;
    }

    // Returns whether the robot in its current state collides with a World
    bool inCollision(const World &world) override {
        return inCollision(mState, world);
    }

    // Static helper that compares an arbitrary state of an N link arm with a world.
    static bool inCollision(const std::vector<double> &state, const World &world) {
        assert(state.size() == N);
        // use world's line collision checker to ensure all links do not collide
        for (Line link : toLineArray(state)) {
            if (world.lineInCollision(link)) return true;
        }
        return false;
    }

    // overrides Drawable's draw() to display the robot
    void draw(Renderer& renderer) override {
        renderer.setColor(0, 0, 0);
        for (Line link : toLineArray(mState)) {
            renderer.drawLine(link);
        }
    }

 private:
    // Private helper for visualization and collision checking
    // Returns a vector of Lines representing each link in 2D coordinates
    static std::vector<Line> toLineArray(const std::vector<double> &state) {
        std::vector<Line> out;
        // start at the root and facing right, add lines starting at endpoint
        // of each previous line segment.
        double curX = ROOT_X, curY = ROOT_Y, curTheta = 0.0;
        for (double theta : state) {
            double nextTheta = curTheta + theta;
            double nextX = curX + LINK_LENGTH * cos(nextTheta);
            double nextY = curY + LINK_LENGTH * sin(nextTheta);
            out.push_back(Line(curX, curY, nextX, nextY));
            curTheta = nextTheta;
            curX = nextX;
            curY = nextY;
        }
        return out;
    }

    // The internal representation of an N-link arm. Each element in the vector
    // is an angle of rotation relative to pointing right (for 0th element) or
    // relative to the direction of the previous link (all others).
    std::vector<double> mState;
};

#endif