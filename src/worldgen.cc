#include "NLinkArm.h"
#include "Visualizer.h"

int main(){//int argc, char** argv) {
    World w();
    NLinkArm<3> arm();
    visualizer::openVisualizer();
    return 0;
}
