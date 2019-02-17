#ifndef _RENDERER_H_
#define _RENDERER_H_
#include <SDL2/SDL.h>
#include "Line.h"
class Renderer {
 public:
    Renderer(SDL_Renderer* sdl) : sdl_renderer(sdl) {}
    void clear();
    void drawLine(const Line& l);
 private:
    Line scaleLine(const Line&);
    SDL_Renderer* sdl_renderer;
};
#endif
