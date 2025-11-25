#pragma once
#include <vector>
#include <memory>
#include "../GameObject.h"

class World {
public:
    void add(std::unique_ptr<GameObject> obj) {
        objects.push_back(std::move(obj));
    }

    void update() {
        for (auto& obj : objects) obj->update();
    }

    void draw() {
        for (auto& obj : objects) obj->draw();
    }

private:
    std::vector<std::unique_ptr<GameObject>> objects;
};
