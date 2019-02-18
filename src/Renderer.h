#ifndef _RENDERER_H_
#define _RENDERER_H_
#include <SDL2/SDL.h>
#include "Line.h"
#include "Rect.h"
class Renderer {
 public:
    Renderer(SDL_Renderer* sdl, int w, int h) : sdl_renderer(sdl), w_(w), h_(h) {}
    void clear();
    void flush();
    void drawLine(const Line& l);
    void fillRect(const Rect& r);
    void setColor(int r, int g, int b);
 private:
    Line scaleLine(const Line&);
    SDL_Rect scaleRect(const Rect&);
    SDL_Renderer* sdl_renderer;
    int w_, h_;
};
#endif
