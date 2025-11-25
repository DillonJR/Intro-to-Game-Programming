#pragma once
#include "../Component.h"

class BodyComponent : public Component {
public:
    BodyComponent(GameObject& parent,
                  float x, float y,
                  float w, float h)
        : Component(parent), x(x), y(y), w(w), h(h) {}

    void update() override {
        x += vx;
        y += vy;
    }

    void draw() override {}

    float x, y, w, h, vx=0, vy=0;
};
