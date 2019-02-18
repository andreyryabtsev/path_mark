#include <SDL2/SDL.h>
#include "Line.h"
#include "Rect.h"
#include "Renderer.h"

void Renderer::flush() {
    SDL_RenderPresent(sdl_renderer);
}

void Renderer::clear() {
    SDL_SetRenderDrawColor(sdl_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(sdl_renderer);
}

void Renderer::setColor(int r, int g, int b) {
    SDL_SetRenderDrawColor(sdl_renderer, r, g, b, SDL_ALPHA_OPAQUE);
}

void Renderer::drawLine(const Line& l) {
    Line sl = scaleLine(l);
    SDL_RenderDrawLine(sdl_renderer, sl.x1, sl.y1, sl.x2, sl.y2);
}

void Renderer::fillRect(const Rect& r) {
    SDL_Rect rect = scaleRect(r);
    SDL_RenderFillRect(sdl_renderer, &rect);
}

Line Renderer::scaleLine(const Line& l) {
    return Line(l.x1 * w_, h_ - l.y1 * h_, l.x2 * w_, h_ - l.y2 * h_);
}

SDL_Rect Renderer::scaleRect(const Rect& r) {
    SDL_Rect ret;
    double top_y = r.y1 > r.y2 ? r.y1 : r.y2;
    ret.x = (int)(r.x1 * w_);
    ret.y = (int)(h_ - top_y * h_);
    ret.w = (int)(abs(r.x2 - r.x1) * w_);
    ret.h = (int)(abs(r.y2 - r.y1) * h_);
    return ret;
}
