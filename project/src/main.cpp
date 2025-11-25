#include <SDL2/SDL.h>
#include "tinyxml2.h"

#include "GameObject.h"
#include "Input.h"
#include "factory/ComponentFactory.h"

int SDL_main(int argc, char** argv)
{
    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Game",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        SDL_Log("CreateWindow Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("CreateRenderer Error: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Input
    Input input;

    // TinyXML
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile("assets/xml/objects.xml") != tinyxml2::XML_SUCCESS) {
        SDL_Log("Failed loading XML file.");
    }

    auto* root = doc.FirstChildElement("Objects");
    std::vector<std::unique_ptr<GameObject>> objects;

    ComponentFactory factory(renderer, input);

    // Build objects from XML
    if (root) {
        for (auto* elem = root->FirstChildElement("Object");
             elem != nullptr;
             elem = elem->NextSiblingElement("Object"))
        {
            auto obj = std::make_unique<GameObject>();
            factory.build(*obj, *elem);
            objects.push_back(std::move(obj));
        }
    }

    bool running = true;
    while (running) {

        // Get inputs
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;

            input.processEvent(e); // YOUR Input system
        }

        // Update objects
        for (auto& obj : objects) {
            obj->update();
        }

        // Draw
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        for (auto& obj : objects) {
            obj->draw();          // FIXED: draw() takes no parameters
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
