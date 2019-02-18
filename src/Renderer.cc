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

void Renderer::drawLine(const Line& l) {
    SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    Line sl = scaleLine(l);
    SDL_RenderDrawLine(sdl_renderer, sl.x1, sl.y1, sl.x2, sl.y2);
}

void Renderer::fillRect(const Rect& r) {
    SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_Rect rect = scaleRect(r);
    SDL_RenderFillRect(sdl_renderer, &rect);
}

Line Renderer::scaleLine(const Line& l) {
    int w, h;
    SDL_GetRendererOutputSize(sdl_renderer, &w, &h);
    return Line(l.x1 * w, h - l.y1 * h, l.x2 * w, h - l.y2 * h);
}

SDL_Rect Renderer::scaleRect(const Rect& r) {
    int w, h;
    SDL_GetRendererOutputSize(sdl_renderer, &w, &h);
    return SDL_Rect{(int)(r.x1 * w),
                    (int)(h - r.y1 * h),
                    (int)((r.x2 - r.x1) * w),
                    (int)((r.y2 - r.y1) * h)};
}
