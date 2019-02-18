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
    NLinkArm() {
        for(int i = 0; i < N; i++) mState.push_back(0.0);
    }

    NLinkArm(std::vector<double> state) {
        assert(state.size() == N);
        mState = state;
    }

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
    void draw(Renderer& renderer) {
        renderer.setColor(0, 0, 0);
        for (Line link : toLineArray(mState)) {
            renderer.drawLine(link);
        }
    }

 private:
    static std::vector<Line> toLineArray(const std::vector<double> &state) {
        std::vector<Line> out;
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

    std::vector<double> mState;
};

#endif