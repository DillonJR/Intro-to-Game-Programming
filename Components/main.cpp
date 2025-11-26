#include "Engine.h"
#include "GameObject.h"
#include "SpriteComponent.h"
#include "BodyComponent.h"
#include "MovementComponent.h"
#include "BackgroundComponent.h"
#include "SlideComponent.h"
#include "FollowComponent.h"
#include "HealthComponent.h"
#include <cstdlib>
#include <ctime>

int main(int argc, char* argv[]) {
    // Initialize random number generator
    srand(static_cast<unsigned int>(time(nullptr)));

    bool running = true;
    while (running) {
        // Initialize the engine
        if (!Engine::init("SDL2 Game Engine", 800, 600)) {
            return -1;
        }

        SDL_Renderer* renderer = Engine::getRenderer();  // Assuming Engine provides a getRenderer method

        // Load textures into the texture manager
        Textures::load("hero", "squirrel.png", renderer);
        Textures::load("enemy", "squirrel.png", renderer);
        Textures::load("projectile", "C:/Users/dillo/Downloads/acorn-png-37309.png", renderer); // Using acorn as projectile

        // Create background game object (drawn first, behind everything)
        // Just drop your PNG file path here - no pre-loading required!
        auto backgroundObject = std::make_unique<GameObject>();
        backgroundObject->add<BackgroundComponent>("C:/Users/dillo/Downloads/squirrelgamebageound.png"); // Your background PNG file
        Engine::addGameObject(std::move(backgroundObject));

        // Create GameObject 1 and add SpriteComponent and BodyComponent
        auto gameObject1 = std::make_unique<GameObject>();
        gameObject1->add<SpriteComponent>("hero");  // Use texture key "hero"
        gameObject1->add<BodyComponent>(100, 200);  // BodyComponent with position (100, 200)
        gameObject1->add<MovementComponent>(200.0f); // MovementComponent with 200 pixels/second
        gameObject1->add<HealthComponent>(1); // Player has 1 health

        // Set hero reference in Engine for enemy respawning
        Engine::setHero(gameObject1.get());

        // Create GameObject 2 and add SpriteComponent and BodyComponent
        auto gameObject2 = std::make_unique<GameObject>();
        gameObject2->add<SpriteComponent>("enemy");  // Use texture key "enemy"
        gameObject2->add<BodyComponent>(300, 400);  // BodyComponent with position (300, 400)
        gameObject2->add<FollowComponent>(*gameObject1, 100.0f); // Follow the hero at 100 pixels/second
        gameObject2->add<HealthComponent>(1); // Enemy has 1 health

        // Add game objects to the engine
        Engine::addGameObject(std::move(gameObject1));
        Engine::addGameObject(std::move(gameObject2));

        // Run the engine, which will handle updating and drawing the game objects
        Engine::run();

        // Check if we need to restart
        if (!Engine::needsRestart) {
            running = false; // Exit the program
        }
        // If needsRestart is true, the loop will continue and reinitialize
    }

    return 0;
}
