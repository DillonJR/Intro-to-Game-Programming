#pragma once
#include "Component.h"

class HealthComponent : public Component {
public:
    HealthComponent(GameObject& parent, int maxHealth = 3)
        : Component(parent), _maxHealth(maxHealth), _currentHealth(maxHealth) {}

    int getCurrentHealth() const { return _currentHealth; }
    int getMaxHealth() const { return _maxHealth; }

    void setHealth(int health) {
        _currentHealth = std::max(0, std::min(health, _maxHealth));
    }

    void takeDamage(int damage = 1) {
        _currentHealth = std::max(0, _currentHealth - damage);
    }

    void heal(int amount = 1) {
        _currentHealth = std::min(_maxHealth, _currentHealth + amount);
    }

    bool isDead() const { return _currentHealth <= 0; }

    void update() override {
        // Health component doesn't need updating
    }

    void draw() override {
        // Health component doesn't draw anything (no hearts display)
    }

private:
    int _maxHealth;
    int _currentHealth;
};
