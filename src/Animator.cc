#include "Visualizer.h"
#define FRAMES 300

void Animator::animate() {
	visualizer::animate(visualizer::openVisualizer(), displayPathAnimationFrame, FRAMES, 16);
}

void Animator::displayPathAnimationFrame(Renderer& r, int i) {
	double m = double(i) / (FRAMES - 1);
}
