#include <SDL2/SDL.h>
#include <iostream>
#include "Visualizer.h"

namespace visualizer {
bool openVisualizer() {
    SDL_Window* window = nullptr;
    SDL_Surface* screenSurface = nullptr;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "could not initialize sdl2: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }
    window = SDL_CreateWindow(
        "PathMark",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        600, 400,
        SDL_WINDOW_SHOWN
        );
    if (window == nullptr) {
        std::cerr << "could not create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }
    screenSurface = SDL_GetWindowSurface(window);
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
    SDL_UpdateWindowSurface(window);
    SDL_Delay(3000);
    return true;
}
}
