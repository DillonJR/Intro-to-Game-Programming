#pragma once
#include "Component.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <memory>
#include "Engine.h"

class BackgroundComponent : public Component {
public:
    BackgroundComponent(GameObject& parent, std::string filePath)
        : Component(parent), texture(nullptr) {
        loadTexture(filePath);
    }

    ~BackgroundComponent() {
        // Texture cleanup handled by unique_ptr
    }

    void update() override {
        // Background doesn't need updating
    }

    void draw() override {
        if (texture) {
            // Get window dimensions
            int windowWidth = Engine::getWindowWidth();
            int windowHeight = Engine::getWindowHeight();

            // Render the background to fill the entire window
            SDL_Rect dst = { 0, 0, windowWidth, windowHeight };
            SDL_RenderCopy(Engine::getRenderer(), texture.get(), nullptr, &dst);
        }
    }

private:
    void loadTexture(const std::string& filePath) {
        SDL_Surface* tempSurface = IMG_Load(filePath.c_str());
        if (!tempSurface) {
            SDL_Log("Failed to load background texture from file: %s, Error: %s", filePath.c_str(), IMG_GetError());
            return;
        }

        // Convert the surface into a texture
        SDL_Texture* rawTexture = SDL_CreateTextureFromSurface(Engine::getRenderer(), tempSurface);
        SDL_FreeSurface(tempSurface);

        if (!rawTexture) {
            SDL_Log("Failed to create background texture from file: %s, Error: %s", filePath.c_str(), SDL_GetError());
            return;
        }

        // Store the texture with custom deleter for automatic cleanup
        texture = std::unique_ptr<SDL_Texture, SDL_Deleter>(rawTexture);
        SDL_Log("Successfully loaded background texture: %s", filePath.c_str());
    }

    // Custom deleter for SDL_Texture
    struct SDL_Deleter {
        void operator()(SDL_Texture* texture) const {
            if (texture) {
                SDL_DestroyTexture(texture);
            }
        }
    };

    std::unique_ptr<SDL_Texture, SDL_Deleter> texture;
};
