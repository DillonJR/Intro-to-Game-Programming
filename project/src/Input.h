#pragma once
#include <SDL.h>
#include <unordered_map>

class Input {
public:
    void beginFrame() {
        keyPressed.clear();
        keyReleased.clear();
    }

    void handleEvent(const SDL_Event& e) {
        if (e.type == SDL_KEYDOWN && !e.key.repeat) {
            keyPressed[e.key.keysym.scancode] = true;
            keyHeld[e.key.keysym.scancode] = true;
        }

        if (e.type == SDL_KEYUP) {
            keyReleased[e.key.keysym.scancode] = true;
            keyHeld[e.key.keysym.scancode] = false;
        }
    }

    bool isHeld(SDL_Scancode key) const {
        auto it = keyHeld.find(key);
        return it != keyHeld.end() && it->second;
    }

private:
    std::unordered_map<SDL_Scancode, bool> keyHeld;
    std::unordered_map<SDL_Scancode, bool> keyPressed;
    std::unordered_map<SDL_Scancode, bool> keyReleased;
};
