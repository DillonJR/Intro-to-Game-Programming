#pragma once
class GameObject;

class Component {
public:
    explicit Component(GameObject& parent) : m_parent(parent) {}
    virtual ~Component() = default;

    virtual void update() = 0;
    virtual void draw() = 0;

    GameObject& parent() { return m_parent; }

protected:
    GameObject& m_parent;
};
