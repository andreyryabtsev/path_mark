/*
A renderer is used to draw graphics for different objects on the visualizer, 
and essentially is an auto-scaling wrapper around SDL's render methods. It will
convert lines, rectangles, etc. from 0.0 to 1.0 coordinates to integer pixel
values appropriate for the window's dimensions while providing a simple interface.
*/
#ifndef _RENDERER_H_
#define _RENDERER_H_
#include <SDL2/SDL.h>
#include "Line.h"
#include "Rect.h"
class Renderer {
 public:
    // Constructs a new renderer using an underlying SDL renderer and dimensions
    Renderer(SDL_Renderer* sdl, int w, int h) : sdl_renderer(sdl), w_(w), h_(h) {}

    // Fills the render canvas with the current color
    void clear();

    // Pushes all the queued renders out onto display. This method should be
    // called whenever the screen is expected to be updated with new changes.
    void flush();

    // Draws a line on the canvas using the current color
    void drawLine(const Line& l);

    // Fills a rectangle on the canvas using the current color
    void fillRect(const Rect& r);

    // Sets the current color to the specified RGB channels
    void setColor(int r, int g, int b);
 private:
    // Private helper to convert a normalized line into a screen-sized line
    Line scaleLine(const Line&);
    // Private helper to convert a normalized Rect into a screen-sized SDL_Rect
    SDL_Rect scaleRect(const Rect&);
    // The SDL_Renderer this Renderer uses for pushing graphics to
    SDL_Renderer* sdl_renderer;
    // The width and height of the window
    int w_, h_;
};
#endif
