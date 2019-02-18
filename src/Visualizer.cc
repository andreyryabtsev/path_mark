#include <SDL2/SDL.h>
#include <iostream>
#include <thread>
#include "Visualizer.h"
#include "World.h"
#include "Renderer.h"

namespace visualizer {

// technical fields for managing window
#define SIZE_X 600
#define SIZE_Y 600
#define EXIT_CODE 0
#define REDRAW_CODE 1
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
bool windowOpen = false;
std::thread *loopThread;
Uint32 eventType;


void _renderLoop();

Renderer openVisualizer() {
    if (windowOpen) throw;
    windowOpen = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "could not initialize sdl2: " << SDL_GetError() << std::endl;
        SDL_Quit();
        throw;
    }
    if(!SDL_CreateWindowAndRenderer(SIZE_X, SIZE_Y, SDL_WINDOW_SHOWN, &window, &renderer) == 0) {
        std::cerr << "could not create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        throw;
    }
    if((eventType = SDL_RegisterEvents(1)) == ((Uint32)-1)) {
        std::cerr << "could not register event: " << SDL_GetError() << std::endl;
        SDL_Quit();
        throw;
    }
    SDL_SetWindowTitle(window, "PathMark");

    loopThread = new std::thread(_renderLoop);

    return Renderer(renderer, SIZE_X, SIZE_Y);
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
    bool loop = true;
    while (loop){
        SDL_Event event;
        if (SDL_WaitEvent(&event)) {
            if (event.type == eventType) {
                switch (event.user.code) {
                    case EXIT_CODE:
                        loop = false;
                        break;
                }
            } else if (event.type == SDL_QUIT) {
                loop = false;
            }
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

}
