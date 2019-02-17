#include <SDL2/SDL.h>
#include <iostream>
#include <thread>
#include "Visualizer.h"
#include "World.h"

namespace visualizer {

// technical fields for managing window
#define SIZE_X 600
#define SIZE_Y 600
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
bool windowOpen = false;
std::thread *loopThread;
Uint32 eventType;

// visualization fields
World world_;


void _renderLoop();

bool openVisualizer() {
    if (windowOpen) return true;
    windowOpen = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "could not initialize sdl2: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }
    if(!SDL_CreateWindowAndRenderer(SIZE_X, SIZE_Y, SDL_WINDOW_SHOWN, &window, &renderer) == 0) {
        std::cerr << "could not create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }
    if((eventType = SDL_RegisterEvents(1)) == ((Uint32)-1)) {
        std::cerr << "could not register event: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    loopThread = new std::thread(_renderLoop);

    return true;
}

void closeVisualizer() {
    SDL_Event e;
    SDL_memset(&e, 0, sizeof(e));
    e.type = eventType;
    e.user.code = 0;
    SDL_PushEvent(&e);
    windowOpen = false;
    if(loopThread->joinable()) {
        loopThread->join();
    }
    delete loopThread;
}

void _renderLoop() {
    SDL_RenderSetScale(renderer, SIZE_X, SIZE_Y);
    bool loop = true;
    while (loop){
        SDL_Event event;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(renderer, 320, 200, 300, 240);
        SDL_RenderDrawLine(renderer, 300, 240, 340, 240);
        SDL_RenderDrawLine(renderer, 340, 240, 320, 200);
        SDL_RenderPresent(renderer);
        if (SDL_WaitEvent(&event))
        {
            if (event.type == eventType) {
                std::cout << "Received client event: code " << event.user.code << std::endl;
                loop = false;
            } else if (event.type == SDL_QUIT) {
                loop = false;
            }
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

}
