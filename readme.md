Concise instructions to use path_mark:

First, make sure SDL2 is installed (`apt-get install libsdl2-dev`)

There are two scripts, unitGraphGenerator.py and worldgen.py. A helper script that can be modified is called prepare. It can be invoked with `./prepare 3` to generate a graph and a world for a 3-link arm, as an example. A link arm has length 0.15, so invoking worldgen.py with min separation (`-s`) < 0.15 has a risk of producing unsolvable worlds. Lowering this value means number of obstacles (`-n`) may need to be lowered to prevent worldgen from being stuck in a loop.

Once a world is generated, the following can be used in a c++ client program:

```c++
// To load a world:
World world("../resources/out/world3.world");

// To check if an N-link arm is in collision with a world:
// where vec_theta is a std::vector of appopriate length, representing theta0, theta1, ... thetaN
// and the first theta is respective to East and remaining thetas are respective to the preceding link
NLinkArm::inCollision(vec_theta, world);

// To use the start and target positions that a world comes with:
std::vector<double> s = world.getStartPosition(),
					t = world.getTargetPosition();

// NLinkArm's, or other Robot's can also be instantiated and altered:
NLinkArm arm(3, world.getStartPosition());
arm.setState(world.getTargetPosition());

// Can also check if an arm instance collides with a world:
arm.inCollision(world);


// To visualize the robot, the visualizer must be opened:
// openVisualizer() opens the window for PathMark and returns a Renderer that draws to it.
Renderer r = visualizer::openVisualizer();
// A renderer can draw all Drawables (World, Robot), and should be flushed at display time:
r.draw(world);
r.draw(arm);
r.flush();
```

Visualizer also helps with animation. Suppose the following function is defined:
```c++
// r as the renderer pre-specified, i is the frame index
void displayPathAnimationFrame(Renderer& r, int i) {
	// use the index i to retreive current animation state
	std::vector<double> vec_state = figureOutPositionAt(i);
	// construct an arm at this state
	NLinkArm arm(DIM, vec_state);
	// draw
	r.draw(animating_world);
	r.draw(arm);
	r.flush();
}
```

Then, can invoke the animation as follows:
```c++
// Using renderer r, call this draw function 133 times about 16 ms apart in separate thread (~60fps)
visualizer::animate(r, displayPathAnimationFrame, 133, 16);
/* do something else or simply wait for animation to finish here*/

// Close the visualization window.
visualizer::closeVisualizer();
```

See `gls_path.cpp` for an example of planning in a world, and then animating the resulting path.

The header files further outline the available functions.
