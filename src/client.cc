#include "NLinkArm.h"
#include "Visualizer.h"
#include "World.h"
#include <iostream>

int main() {
    World w("../resources/worlds/test.world");
    std::vector<double> initState;
    double initStateArray[3] = {0.1, 1.2, -0.7};
    initState.assign(initStateArray, initStateArray + 3);
    NLinkArm<3> arm(initState);
    Renderer r = visualizer::openVisualizer();
    r.draw(w);
    r.draw(arm);
    r.flush();

    std::cin.get();
    visualizer::closeVisualizer();

    return 0;
}
