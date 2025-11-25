#pragma once
#include <SDL2/SDL.h>

class Input {
public:
    void processEvent(const SDL_Event& e) {
        if (e.type == SDL_KEYDOWN) keys[e.key.keysym.scancode] = true;
        if (e.type == SDL_KEYUP) keys[e.key.keysym.scancode] = false;
    }

    bool isHeld(SDL_Scancode scan) const {
        return keys[scan];
    }

private:
    bool keys[SDL_NUM_SCANCODES] = {};
};
