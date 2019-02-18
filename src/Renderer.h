#ifndef _RENDERER_H_
#define _RENDERER_H_
#include <SDL2/SDL.h>
#include "Line.h"
#include "Rect.h"
class Renderer {
 public:
    Renderer(SDL_Renderer* sdl) : sdl_renderer(sdl) {}
    void clear();
    void flush();
    void drawLine(const Line& l);
    void fillRect(const Rect& r);
 private:
    Line scaleLine(const Line&);
    SDL_Rect scaleRect(const Rect&);
    SDL_Renderer* sdl_renderer;
};
#endif
