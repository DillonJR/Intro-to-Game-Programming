#include <SDL2/SDL.h>
#include "Input.h"
#include "world/World.h"
#include "factory/ComponentFactory.h"
#include "tinyxml2.h"
#include "DebugConfig.h"

bool gDebugDraw = true; // enables debug rectangles

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "Component Demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Input input;
    World world;

    // Load XML
    tinyxml2::XMLDocument doc;
    doc.LoadFile("assets/xml/objects.xml");

    auto root = doc.RootElement();
    ComponentFactory factory(renderer, input);

    for (auto objElem = root->FirstChildElement("GameObject");
         objElem != nullptr;
         objElem = objElem->NextSiblingElement("GameObject"))
    {
        auto obj = std::make_unique<GameObject>();
        factory.build(*obj, *objElem);
        world.add(std::move(obj));
    }

    bool running = true;
    SDL_Event e;

    while (running) {
        input.beginFrame();

        while (SDL_PollEvent(&e)) {

            if (e.type == SDL_QUIT)
                running = false;

            if (e.type == SDL_KEYDOWN && !e.key.repeat) {
                if (e.key.keysym.scancode == SDL_SCANCODE_F1) {
                    gDebugDraw = !gDebugDraw;
                }
            }

            input.handleEvent(e);
        }

        world.update();

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        world.draw();

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
