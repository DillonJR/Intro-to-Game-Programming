#pragma once
#include "Component.h"
#include "BodyComponent.h"
#include <cmath>

class ProjectileComponent : public Component {
public:
    ProjectileComponent(GameObject& parent, float speed = 300.0f, float directionX = 0.0f, float directionY = -1.0f)
        : Component(parent), _speed(speed), _dirX(directionX), _dirY(directionY), _lifetime(0.0f), _maxLifetime(5.0f) {
        // Normalize direction if not already normalized
        float length = std::sqrt(_dirX * _dirX + _dirY * _dirY);
        if (length > 0.0f) {
            _dirX /= length;
            _dirY /= length;
        }
    }

    void update() override {
        auto body = parent().get<BodyComponent>();
        if (!body) return;

        // Move in the specified direction
        body->x() += _dirX * _speed * 0.016f; // Approximate 60 FPS delta time
        body->y() += _dirY * _speed * 0.016f;

        // Update lifetime
        _lifetime += 0.016f;

        // Destroy projectile if it goes off screen or exceeds lifetime
        if (_lifetime > _maxLifetime ||
            body->x() < -100 || body->x() > 900 ||  // Off screen bounds
            body->y() < -100 || body->y() > 700) {  // Off screen bounds
            // Mark for destruction - we'll need to handle this in the Engine
            _shouldDestroy = true;
        }
    }

    void draw() override {
        // Projectile component doesn't draw - SpriteComponent handles that
    }

    bool shouldDestroy() const { return _shouldDestroy; }
    void destroy() { _shouldDestroy = true; }

private:
    float _speed;      // pixels per second
    float _dirX, _dirY; // normalized direction vector
    float _lifetime;    // how long the projectile has existed
    float _maxLifetime; // maximum lifetime in seconds
    bool _shouldDestroy = false;
};
