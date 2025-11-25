#pragma once
#include "../Component.h"
#include "../Input.h"

class ControllerComponent : public Component {
public:
    ControllerComponent(GameObject& parent, Input& i, float s)
        : Component(parent), input(i), speed(s) {}

    void update(float dt) override {
        if (input.left)  m_parent.x -= speed * dt;
        if (input.right) m_parent.x += speed * dt;
        if (input.up)    m_parent.y -= speed * dt;
        if (input.down)  m_parent.y += speed * dt;
    }

    // REQUIRED: fix abstract class error
    void draw() override {
        // Controller does not draw anything
    }

private:
    Input& input;
    float speed;
};
