#include "NLinkArm.h"
#include "Visualizer.h"
#include "World.h"
#include <unistd.h>
#include <iostream>

#define DIM 2
#define ANIM 100

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
	r.draw(w);
	r.draw(arm);
	r.flush();
}

int main() {
    w = World("../resources/out/world_out.world");
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
