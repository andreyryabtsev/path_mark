/*
Define a collection of methods for managing starting and opening a visualization
window.
*/
#ifndef _VISUALIZER_H_
#define _VISUALIZER_H_
#include "Renderer.h"
namespace visualizer {
// Opens a visualizer if none already open and returns a Renderer object to
// push graphics to the window. Throws an exception if window already open
Renderer openVisualizer();

// Starts an animation using renderer r, for specified number of frames with a certain delay
// after each frame. The draw function should take in a renderer and a frame index
// if called after previous animation has not completed, will wait on previous animation to complete
// before starting this one (and immediately returning)
void animate(Renderer& r, void (*fDraw)(Renderer&, int), int frames, int delay);

// Closes the current visualizer window and cleans up its thread
void closeVisualizer();
}
#endif