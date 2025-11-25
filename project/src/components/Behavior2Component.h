#pragma once
#include "../Component.h"

class Behavior2Component : public Component {
public:
    Behavior2Component(GameObject& parent)
        : Component(parent) {}

    void update() override {}

    void draw() override {}
};
