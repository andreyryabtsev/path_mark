#include "NLinkArm.h"
#include "Visualizer.h"
#include "Renderer.h"
#include "World.h"
#include <iostream>

int main() {
    World w("/home/andrey/projects/path_mark/resources/worlds/test.world");
    std::vector<double> initState;
    double initStateArray[3] = {0.1, 0.5, -0.2};
    initState.assign(initStateArray, initStateArray + 3);
    NLinkArm<3> arm(initState);
    Renderer r = visualizer::openVisualizer();
    w.draw(r);
    arm.draw(r);
    r.flush();

    std::cin.get();
    visualizer::closeVisualizer();

    return 0;
}
