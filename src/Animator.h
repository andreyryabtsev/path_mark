/*
An animator facilitates animation of an instance of ompl::geometric::PathGeometric.
*/
#ifndef _ANIMATOR_H_
#define _ANIMATOR_H_
class Animator {
 public:
    // Constructs a new animator with a path to animate
    Animator(std::shared_ptr<ompl::geometric::PathGeometric> path_ptr, void (*fDraw)(Renderer&, std::vector<double>))
        : mPath(path_ptr), mRenderer(r), mDrawFunction(fDraw) {
            initializeAnimationInfo();
        }
    // Starts the animation
    void animate();
 private:
    // The draw function to pass to generic visualizer animator
    void displayPathAnimationFrame(Renderer& r, int i);
    // Prepares information for animation
    void initializeAnimationInfo();

    // The supplied draw function pointer
    void (*fDraw)(Renderer&, std::vector<double>) mDrawFunction;
    // The path to follow
    std::shared_ptr<ompl::geometric::PathGeometric> mPath;
    //
};
#endif
