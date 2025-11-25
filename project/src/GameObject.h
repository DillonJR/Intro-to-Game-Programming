#pragma once
#include <vector>
#include <memory>
#include <type_traits>
#include <typeinfo>
#include "Component.h"

class GameObject {
public:
    GameObject() = default;

    template <typename T, typename... Args>
    T& add(Args&&... args) {
        auto comp = std::make_unique<T>(*this, std::forward<Args>(args)...);
        T& ref = *comp;
        m_components.push_back(std::move(comp));
        return ref;
    }

    template <typename T>
    T* get() {
        for (auto& c : m_components) {
            if (auto ptr = dynamic_cast<T*>(c.get()))
                return ptr;
        }
        return nullptr;
    }

    void update() {
        for (auto& c : m_components) c->update();
    }

    void draw() {
        for (auto& c : m_components) c->draw();
    }

private:
    std::vector<std::unique_ptr<Component>> m_components;
};
