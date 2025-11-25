#pragma once
#include <string>
#include <map>
#include <functional>

#include "../GameObject.h"
#include "../Input.h"

#include "../components/BodyComponent.h"
#include "../components/SpriteComponent.h"
#include "../components/ControllerComponent.h"
#include "../components/Behavior1Component.h"
#include "../components/Behavior2Component.h"

class ComponentFactory {
public:
    ComponentFactory(SDL_Renderer* r, Input& i)
        : renderer(r), input(i) {
        registerAll();
    }

    void build(GameObject& obj, const tinyxml2::XMLElement& elem) {
        for (auto c = elem.FirstChildElement(); c != nullptr; c = c->NextSiblingElement()) {
            std::string name = c->Name();
            auto it = registry.find(name);
            if (it != registry.end()) it->second(obj, *c);
        }
    }

private:
    SDL_Renderer* renderer;
    Input& input;

    using CreateFn = std::function<void(GameObject&, const tinyxml2::XMLElement&)>;
    std::map<std::string, CreateFn> registry;

    void registerAll() {
        registry["BodyComponent"] =
            [](GameObject& obj, const tinyxml2::XMLElement& e) {
                obj.add<BodyComponent>(
                    e.FloatAttribute("x"),
                    e.FloatAttribute("y"),
                    e.FloatAttribute("width"),
                    e.FloatAttribute("height")
                );
        };

        registry["SpriteComponent"] =
            [this](GameObject& obj, const tinyxml2::XMLElement&) {
                SDL_Texture* tex = nullptr;
                obj.add<SpriteComponent>(renderer, tex);
        };

        registry["ControllerComponent"] =
        [this](GameObject& obj, const tinyxml2::XMLElement& elem)
    {
        float speed = elem.FloatAttribute("speed", 100.0f);
        obj.add<ControllerComponent>(input, speed);
    };

        registry["Behavior1Component"] =
            [](GameObject& obj, const tinyxml2::XMLElement&) {
                obj.add<Behavior1Component>();
        };

        registry["Behavior2Component"] =
            [](GameObject& obj, const tinyxml2::XMLElement&) {
                obj.add<Behavior2Component>();
        };
    }
};
