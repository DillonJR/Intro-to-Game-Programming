#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <memory>  // For std::unique_ptr
#include <algorithm>  // For std::remove_if
#include <utility>    // For std::pair
#include <cstdlib>    // For rand()
#include <ctime>      // For time()
#include "Input.h"
#include "GameObject.h"
#include "ProjectileComponent.h"
#include "HealthComponent.h"
#include "FollowComponent.h"
#include "BodyComponent.h"
#include "MovementComponent.h"

class Engine {
public:
    static int width;
    static int height;
    static bool gameOver;
    static bool needsRestart;

    // Initialize the Engine (static)
    static bool init(const char* title, int width, int height) {
        Engine::width = width;
        Engine::height = height;
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
            return false;
        }

        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
        if (!window) {
            SDL_Log("Failed to create window: %s", SDL_GetError());
            return false;
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            SDL_Log("Failed to create renderer: %s", SDL_GetError());
            return false;
        }

        isRunning = true;
        return true;
    }

    // Handle events (static)
    static void handleEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
            Input::processEvent(event);  // Call static Input method
        }
    }

    // Check for collisions between projectiles, enemies, and hero
    static void checkCollisions() {
        // First check projectile-enemy collisions
        for (size_t i = 0; i < gameObjects.size(); ++i) {
            auto projectile = gameObjects[i]->get<ProjectileComponent>();
            if (!projectile) continue;

            auto projBody = gameObjects[i]->get<BodyComponent>();
            if (!projBody) continue;

            for (size_t j = 0; j < gameObjects.size(); ++j) {
                if (i == j) continue; // Don't check self

                auto health = gameObjects[j]->get<HealthComponent>();
                auto follow = gameObjects[j]->get<FollowComponent>(); // Only enemies have FollowComponent
                if (!health || !follow) continue;

                auto enemyBody = gameObjects[j]->get<BodyComponent>();
                if (!enemyBody) continue;

                // Simple rectangular collision detection (32x32 pixel sprites)
                float projLeft = projBody->x();
                float projRight = projBody->x() + 32;
                float projTop = projBody->y();
                float projBottom = projBody->y() + 32;

                float enemyLeft = enemyBody->x();
                float enemyRight = enemyBody->x() + 32;
                float enemyTop = enemyBody->y();
                float enemyBottom = enemyBody->y() + 32;

                if (projRight > enemyLeft && projLeft < enemyRight &&
                    projBottom > enemyTop && projTop < enemyBottom) {
                    // Collision detected!
                    health->takeDamage(1); // Damage the enemy
                    projectile->destroy(); // Mark projectile for destruction
                    break; // Projectile can only hit one enemy
                }
            }
        }

        // Now check hero-enemy collisions
        for (size_t i = 0; i < gameObjects.size(); ++i) {
            // Check if this is the hero (has MovementComponent and HealthComponent)
            auto movement = gameObjects[i]->get<MovementComponent>();
            auto heroHealth = gameObjects[i]->get<HealthComponent>();
            auto heroBody = gameObjects[i]->get<BodyComponent>();
            if (!movement || !heroHealth || !heroBody) continue;

            for (size_t j = 0; j < gameObjects.size(); ++j) {
                if (i == j) continue; // Don't check self

                // Check if this is an enemy (has FollowComponent)
                auto follow = gameObjects[j]->get<FollowComponent>();
                auto enemyBody = gameObjects[j]->get<BodyComponent>();
                if (!follow || !enemyBody) continue;

                // Simple rectangular collision detection (32x32 pixel sprites)
                float heroLeft = heroBody->x();
                float heroRight = heroBody->x() + 32;
                float heroTop = heroBody->y();
                float heroBottom = heroBody->y() + 32;

                float enemyLeft = enemyBody->x();
                float enemyRight = enemyBody->x() + 32;
                float enemyTop = enemyBody->y();
                float enemyBottom = enemyBody->y() + 32;

                if (heroRight > enemyLeft && heroLeft < enemyRight &&
                    heroBottom > enemyTop && heroTop < enemyBottom) {
                    // Hero-enemy collision detected - damage the hero!
                    heroHealth->takeDamage(1);
                    SDL_Log("Player hit by enemy! Health: %d", heroHealth->getCurrentHealth());

                    // Check if player is dead
                    if (heroHealth->isDead()) {
                        SDL_Log("GAME OVER! Player died!");
                        gameOver = true;
                    }
                    break; // Only one collision per enemy per frame
                }
            }
        }
    }

    // Update all game objects (static)
    static void update() {
        if (gameOver) return; // Don't update if game is over

        for (auto& gameObject : gameObjects) {
            gameObject->update();  // Update each GameObject
        }

        // Check for collisions between projectiles and enemies
        checkCollisions();

        // Remove destroyed projectiles and dead enemies
        bool enemyDied = false;
        gameObjects.erase(
            std::remove_if(gameObjects.begin(), gameObjects.end(),
                [&enemyDied](const std::unique_ptr<GameObject>& obj) {
                    // Check if this object has a projectile component that should be destroyed
                    auto projectile = obj->get<ProjectileComponent>();
                    if (projectile && projectile->shouldDestroy()) {
                        return true;
                    }

                    // Check if this object has a health component and is dead
                    auto health = obj->get<HealthComponent>();
                    if (health && health->isDead()) {
                        // Check if this is an enemy (has FollowComponent)
                        auto follow = obj->get<FollowComponent>();
                        if (follow) {
                            enemyDied = true;
                        }
                        return true;
                    }
                    return false;
                }),
            gameObjects.end());

        // Respawn enemy if one died (after vector operations are complete)
        if (enemyDied) {
            createEnemy();
        }
    }

    // Render all game objects (static)
    static void render() {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Always draw game objects (background, etc.)
        for (auto& gameObject : gameObjects) {
            gameObject->draw();  // Draw each GameObject
        }

        if (gameOver) {
            // Enable alpha blending for transparency
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

            // Draw semi-transparent dark overlay over entire screen
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128); // Black with 50% transparency
            SDL_Rect fullScreenRect = {0, 0, width, height};
            SDL_RenderFillRect(renderer, &fullScreenRect);

            // Draw white game over box
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Solid white
            SDL_Rect overlayRect = {width/2 - 150, height/2 - 50, 300, 100};
            SDL_RenderFillRect(renderer, &overlayRect);

            // Draw border
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black border
            SDL_RenderDrawRect(renderer, &overlayRect);

            // Reset blend mode
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        }

        SDL_RenderPresent(renderer);
    }

    // Clean up SDL resources (static)
    static void clean() {
        if (renderer) {
            SDL_DestroyRenderer(renderer);
        }
        if (window) {
            SDL_DestroyWindow(window);
        }
        SDL_Quit();
    }

    // Check if the engine is still running (static)
    static bool running() {
        return isRunning;
    }

    // Add a GameObject to the engine (static)
    static void addGameObject(std::unique_ptr<GameObject> gameObject) {
        gameObjects.push_back(std::move(gameObject));  // Add the game object to the list
    }

    // Create a projectile GameObject immediately (static)
    static void createProjectile(float x, float y, float dirX, float dirY, float speed = 300.0f);

    // Create an enemy GameObject at random location (static)
    static void createEnemy();

    // Set the hero reference for enemy following (static)
    static void setHero(GameObject* heroRef) { hero = heroRef; }

    // Run the engine (static)
    static void run() {
        while (isRunning) {
            handleEvents();

            if (!gameOver) {
                update();
            } else {
                // Game over state - show options
                handleGameOver();
            }

            render();
            SDL_Delay(16);

            // Check if restart was requested
            if (needsRestart) {
                needsRestart = false;
                return; // Exit run() to allow main.cpp to restart
            }
        }

        clean();
    }

    // Handle game over state
    static void handleGameOver() {
        // For now, just stop the game when R is pressed to restart or Q to quit
        // In a real game, you'd show text on screen
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_r) {
                    // Restart game
                    restartGame();
                } else if (event.key.keysym.sym == SDLK_q || event.key.keysym.sym == SDLK_ESCAPE) {
                    // Quit game
                    isRunning = false;
                }
            } else if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }
    }

    // Restart the game
    static void restartGame() {
        gameOver = false;
        // Clear all game objects and reinitialize
        gameObjects.clear();
        hero = nullptr;

        // Set a flag to indicate restart is needed
        needsRestart = true;
        SDL_Log("Press R to restart or Q to quit!");
    }

    static SDL_Renderer* getRenderer() {
        return renderer;
    }

    static int getWindowWidth() {
        return width;
    }

    static int getWindowHeight() {
        return height;
    }

private:
    static bool isRunning;                               // Engine running state (static)
    static SDL_Window* window;                           // SDL window (static)
    static SDL_Renderer* renderer;                       // SDL renderer (static)
    static std::vector<std::unique_ptr<GameObject>> gameObjects;  // Track game objects
    static GameObject* hero;                             // Reference to hero GameObject
};
