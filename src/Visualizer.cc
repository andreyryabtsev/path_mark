#include <SDL2/SDL.h>
#include <iostream>
#include <thread>
#include "Visualizer.h"

namespace visualizer {

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
bool windowOpen = false;
std::thread *loopThread;
void _renderLoop();

bool openVisualizer() {
    if (windowOpen) return true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "could not initialize sdl2: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }
    if(!SDL_CreateWindowAndRenderer(600, 400, SDL_WINDOW_SHOWN, &window, &renderer) == 0) {
        std::cerr << "could not create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    loopThread = new std::thread(_renderLoop);

    return true;
}

void _renderLoop() {
    bool loop = true;
    while (loop){
        SDL_Event event;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(renderer, 320, 200, 300, 240);
        SDL_RenderDrawLine(renderer, 300, 240, 340, 240);
        SDL_RenderDrawLine(renderer, 340, 240, 320, 200);
        SDL_RenderPresent(renderer);
        if (SDL_WaitEvent(&event)) /* execution suspends here while waiting on an event */
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    loop = false;
            }
        }
    }
    SDL_Quit();
}

}
