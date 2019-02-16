#ifndef _N_LINK_ARM_H_

#include <assert.h>
#include <math.h>
#include "Robot.h"
#include "Line.h"

#define LINK_LENGTH 1.0

template <int N>
class NLinkArm : public Robot {
 public:
    bool inCollision(const std::vector<double> &state, const World &world) override {
        assert(state.size() == N);
        // Assuming for the moment that each link is 1.00 long
        double curX = 0.0, curY = 0.0, curTheta = 0.0;
        for (double theta : state) {
            double nextTheta = curTheta + theta;
            double nextX = curX + LINK_LENGTH * cos(nextTheta);
            double nextY = curY + LINK_LENGTH * sin(nextTheta);
            if (world.lineInCollision(Line(curX, curY, nextX, nextY))) return true;
            curTheta = nextTheta;
            curX = nextX;
            curY = nextY;
        }
        return true;
    }
 private:
    double mState[N];
};

#endif