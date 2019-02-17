#ifndef _N_LINK_ARM_H_
#define _N_LINK_ARM_H_

#include <assert.h>
#include <math.h>
#include "Robot.h"
#include "Line.h"

#define LINK_LENGTH 1.0

template <int N>
class NLinkArm : public Robot {
 public:
    bool inCollision(const World &world) override {
        return inCollision(mState, world);
    }
    static bool inCollision(const std::vector<double> &state, const World &world) {
        assert(state.size() == N);
        for (Line link : toLineArray(state)) {
            if (world.lineInCollision(link)) return true;
        }
        return false;
    }
 private:
    static std::vector<Line> toLineArray(const std::vector<double> &state) {
        std::vector<Line> out;
        double curX = 0.0, curY = 0.0, curTheta = 0.0;
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

    std::vector<double> mState;
};

#endif