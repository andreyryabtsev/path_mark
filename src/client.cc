#include "NLinkArm.h"
#include "Visualizer.h"
#include "Renderer.h"
#include "World.h"
#include <iostream>

int main(){//int argc, char** argv) {
    World w;
    NLinkArm<3> arm;
    Renderer r = visualizer::openVisualizer();
    w.draw(r);
    int a;
    std::cin >> a;
    visualizer::closeVisualizer();

    return 0;
}
