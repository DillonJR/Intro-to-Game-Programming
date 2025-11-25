#pragma once
#include "../Component.h"
#include "../Input.h"
#include "../components/BodyComponent.h"

class ControllerComponent : public Component {
public:
    ControllerComponent(GameObject& parent, Input& i, float speed)
        : Component(parent), input(i), speed(speed) {}

    void update() override {
        auto body = parent().get<BodyComponent>();
        if (!body) return;

        body->vx = 0;
        body->vy = 0;

        if (input.isHeld(SDL_SCANCODE_W)) body->vy -= speed;
        if (input.isHeld(SDL_SCANCODE_S)) body->vy += speed;
        if (input.isHeld(SDL_SCANCODE_A)) body->vx -= speed;
        if (input.isHeld(SDL_SCANCODE_D)) body->vx += speed;
    }

    void draw() override {}

private:
    Input& input;
    float speed;
};
