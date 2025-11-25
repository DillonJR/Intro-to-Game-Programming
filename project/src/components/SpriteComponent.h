#pragma once
#include "../Component.h"
#include "../components/BodyComponent.h"
#include "../components/Behavior1Component.h"
#include "../DebugConfig.h"
#include <SDL2/SDL.h>

class SpriteComponent : public Component {
public:
    SpriteComponent(GameObject& parent, SDL_Renderer* r, SDL_Texture* t)
        : Component(parent), renderer(r), texture(t) {}

    void update() override {}

    void draw() override {
        if (!renderer) return;

        auto body = parent().get<BodyComponent>();
        if (!body) return;

        SDL_Rect dst = {
            (int)body->x,
            (int)body->y,
            (int)body->w,
            (int)body->h
        };

        if (gDebugDraw || texture == nullptr) {
            // Enemy (has Behavior1Component) -> RED
            if (parent().get<Behavior1Component>())
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            else
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

            SDL_RenderFillRect(renderer, &dst);
            return;
        }

        SDL_RenderCopy(renderer, texture, nullptr, &dst);
    }

private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
};
