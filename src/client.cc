#include "NLinkArm.h"
#include "Visualizer.h"
#include "World.h"
#include <unistd.h>
#include <iostream>
#include <sstream>

#define DIM 4
#define ANIM 300

NLinkArm<DIM> arm;
World w;

std::vector<double> getInterpolation(const std::vector<double>& start, const std::vector<double>& end, double m) {
	std::vector<double> tmp;
    for(int i = 0; i < DIM; i++) {
    	tmp.push_back(start[i] + m * (end[i] - start[i]));
    }
    return tmp;
}

void animate(Renderer& r, int i) {
	i++;
	arm.setState(getInterpolation(w.getStartPosition(), w.getTargetPosition(), i / (double)ANIM));
    std::cout << arm.inCollision(w);
    std::cout.flush();
	r.draw(w);
	r.draw(arm);
	r.flush();
}

int main() {
    std::stringstream ss;
    ss << "../resources/out/world" << DIM << ".world";
    w = World(ss.str());
    arm = NLinkArm<DIM>(w.getStartPosition());
    Renderer r = visualizer::openVisualizer();
    r.draw(w);
    r.draw(arm);
    r.flush();

 	visualizer::animate(r, animate, ANIM, 16); // ~60 fps 

    std::cin.get();
    visualizer::closeVisualizer();

    return 0;
}
