#include "NLinkArm.h"
#include "Visualizer.h"
#include "World.h"
#include <iostream>

int main() {
    World w("../resources/out/world_out.world");
    NLinkArm<2> arm(w.getStartPosition());
    Renderer r = visualizer::openVisualizer();
    r.draw(w);
    r.draw(arm);
    r.flush();

    std::cin.get();
    visualizer::closeVisualizer();

    return 0;
}
