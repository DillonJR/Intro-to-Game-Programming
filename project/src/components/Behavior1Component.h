#pragma once
#include "../Component.h"

class Behavior1Component : public Component {
public:
    Behavior1Component(GameObject& parent)
        : Component(parent) {}

    void update() override {
        // Simple enemy idle or patrol can go here later
    }

    void draw() override {}
};
