#include "NLinkArm.h"
#include "Visualizer.h"
#include "World.h"
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <boost/program_options.hpp>

#define ANIM 150
static unsigned int DIM = 3;

NLinkArm *arm;
World w;

std::vector<double> getInterpolation(const std::vector<double>& start, const std::vector<double>& end, double m) {
	std::vector<double> tmp;
    for(int i = 0; i < DIM; i++) {
    	tmp.push_back(start[i] + m * (end[i] - start[i]));
    }
    return tmp;
}

void animate(std::shared_ptr<Renderer> r, int i) {
	i++;
	arm->setState(getInterpolation(w.getStartPosition(), w.getTargetPosition(), i / (double)ANIM));
    std::cout << arm->inCollision(w);
    std::cout.flush();
	r->draw(w);
	r->draw(*arm);
	r->flush();
}

int main() {
    // po::options_description desc("2D Map Test Options");
    // desc.add_options()
    //   ("dimension,d", po::value<int>()->default_value(-1), "dimension count")
    // ;

    // // Read arguments
    // po::variables_map vm;
    // po::store(po::parse_command_line(argc, argv, desc), vm);
    // po::notify(vm);
    // DIM = max(DIM, vm["dimension"].as<int>());

    std::stringstream ss;
    ss << "../resources/out/world" << DIM << ".world";
    w = World(ss.str());
    arm = new NLinkArm(DIM, w.getStartPosition());
    std::shared_ptr<Renderer> r = visualizer::openVisualizer();
    r->draw(w);
    r->draw(*arm);
    r->flush();

 	visualizer::animate(r, animate, ANIM, 16); // ~60 fps 

    std::cin.get();
    visualizer::closeVisualizer();

    return 0;
}
