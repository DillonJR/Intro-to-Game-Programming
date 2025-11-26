#pragma once
#include "Component.h"
#include "BodyComponent.h"
#include "GameObject.h"
#include <cmath>

class FollowComponent : public Component {
public:
    FollowComponent(GameObject& parent, GameObject& target, float speed = 150.0f)
        : Component(parent), _target(target), _speed(speed) {}

    void update() override {
        auto body = parent().get<BodyComponent>();
        auto targetBody = _target.get<BodyComponent>();

        if (!body || !targetBody) return;

        // Calculate direction towards target
        float dx = targetBody->x() - body->x();
        float dy = targetBody->y() - body->y();

        // Calculate distance
        float distance = std::sqrt(dx * dx + dy * dy);

        // If we're not at the target, move towards it
        if (distance > 1.0f) {  // Small threshold to prevent jittering
            // Normalize direction
            dx /= distance;
            dy /= distance;

            // Move towards target (assuming ~60 FPS, so multiply by ~0.016)
            body->x() += dx * _speed * 0.016f;
            body->y() += dy * _speed * 0.016f;
        }
    }

    void draw() override {
        // Follow component doesn't need to draw anything
    }

private:
    GameObject& _target;
    float _speed; // pixels per second
};
