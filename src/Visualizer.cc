#include <SDL2/SDL.h>
#include <iostream>
#include <thread>
#include "Visualizer.h"
#include "World.h"
#include "Renderer.h"
#include "Animator.h"
#include <unistd.h>

namespace visualizer {

// constants for internal events and default window size
#define SIZE_X 1000
#define SIZE_Y 1000
#define EXIT_CODE 0
#define REDRAW_CODE 1

// technical fields for managing window in a separate thread
// and appropriately closing down when closeVisualizer() is called
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
bool windowOpen = false;
std::thread *loopThread = nullptr, *animationThread = nullptr;
Uint32 eventType;


void _renderLoop();

std::shared_ptr<Renderer> openVisualizer() {
    if (windowOpen) throw; // currently only support one window at a time
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
    SDL_SetWindowPosition(window, 2000, SDL_WINDOWPOS_UNDEFINED);
    if((eventType = SDL_RegisterEvents(1)) == ((Uint32)-1)) {
        std::cerr << "could not register event: " << SDL_GetError() << std::endl;
        SDL_Quit();
        throw;
    }
    // brief initialization
    SDL_SetWindowTitle(window, "PathMark");

    // start the renderer as a separate thread, so program execution can continue
    // undisturbed. store loopThread on the heap, so it can be joined on shutdown
    loopThread = new std::thread(_renderLoop);

    // return a new renderer wrapping SDL's renderer
    return std::make_shared<Renderer>(renderer, SIZE_X, SIZE_Y);
}

void closeVisualizer() {
    // create and push SDL custom event telling _renderLoop() thread to finish
    // execution, update global variable on window state
    SDL_Event e;
    SDL_memset(&e, 0, sizeof(e));
    e.type = eventType;
    e.user.code = 0;
    SDL_PushEvent(&e);
    windowOpen = false;
    // If the loop thread is still running, join it ensuring window closes
    if(loopThread->joinable()) {
        loopThread->join();
    }
    if(animationThread->joinable()) {
        animationThread->join();
    }
    // thread completed execution, free it
    delete loopThread;
}

void _animate(std::shared_ptr<Renderer> r, void (*fDraw)(std::shared_ptr<Renderer>, int), int frames, int delay) {
    for(int i = 0; i < frames; i++) {
        fDraw(r, i);
        usleep(delay * 1000);        
    }
}

void _animate2(std::shared_ptr<Renderer> r, Animator& a, int frames, int delay) {
    for(int i = 0; i < frames; i++) {
        a.draw(r, i);
        //std::cout << "frames = " << frames << ", delay = " << delay << ", i = " << i << std::endl;
        usleep(delay * 1000);        
    }
}

void animate(std::shared_ptr<Renderer> r, void (*fDraw)(std::shared_ptr<Renderer>, int), int frames, int delay) {
    if(animationThread != nullptr) {
        if(animationThread->joinable()) {
            animationThread->join();
        }
        delete animationThread;
    }
    animationThread = new std::thread(_animate, r, fDraw, frames, delay);
}

void animate(std::shared_ptr<Renderer> r, Animator& a, int frames, int delay) {
    if(animationThread != nullptr) {
        if(animationThread->joinable()) {
            animationThread->join();
        }
        delete animationThread;
    }
    animationThread = new std::thread(_animate2, r, std::ref(a), frames, delay);
}

void _renderLoop() {
    bool loop = true;
    while (loop){
        SDL_Event event;
        // hold execution, waiting on an event (currently only QUIT)
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
    // quit signal was issued. clean up renderer/window and complete execution
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

}
