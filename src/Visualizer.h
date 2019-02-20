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

// Closes the current visualizer window and cleans up its thread
void closeVisualizer();
}
#endif