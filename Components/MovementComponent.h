#pragma once
#include "Component.h"
#include "BodyComponent.h"
#include "Input.h"
#include <SDL2/SDL.h>

// Forward declaration of global function
void createProjectile(float x, float y, float dirX, float dirY, float speed = 300.0f);
#include <SDL2/SDL.h>

class MovementComponent : public Component {
public:
    MovementComponent(GameObject& owner, float speed = 200.0f)
        : Component(owner), _speed(speed) {}

    void update() override {
        auto body = parent().get<BodyComponent>();
        if (!body) return;

        // Handle WASD movement
        if (Input::isKeyDown(SDLK_w) || Input::isKeyDown(SDLK_UP)) {
            body->y() -= _speed * 0.016f; // Approximate 60 FPS delta time
        }
        if (Input::isKeyDown(SDLK_s) || Input::isKeyDown(SDLK_DOWN)) {
            body->y() += _speed * 0.016f;
        }
        if (Input::isKeyDown(SDLK_a) || Input::isKeyDown(SDLK_LEFT)) {
            body->x() -= _speed * 0.016f;
        }
        if (Input::isKeyDown(SDLK_d) || Input::isKeyDown(SDLK_RIGHT)) {
            body->x() += _speed * 0.016f;
        }

        // Handle space bar shooting (single shot on press)
        static bool spaceWasPressed = false;
        bool spacePressed = Input::isKeyDown(SDLK_SPACE);

        if (spacePressed && !spaceWasPressed) {
            // Space was just pressed - shoot projectile upward
            createProjectile(body->x() + 10, body->y(), 0.0f, -1.0f, 300.0f);
        }

        spaceWasPressed = spacePressed;
    }

    void draw() override {
        // Movement component doesn't need to draw anything
    }

private:
    float _speed; // pixels per second
};
