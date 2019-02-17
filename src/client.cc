#include "NLinkArm.h"
#include "Visualizer.h"
#include <iostream>

int main(){//int argc, char** argv) {
    World w();
    NLinkArm<3> arm();
    visualizer::openVisualizer();
    int a;
    std::cin >> a;

    return 0;
}
