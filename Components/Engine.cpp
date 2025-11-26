#include "Engine.h"
#include "SpriteComponent.h"
#include "BodyComponent.h"
#include "FollowComponent.h"
#include "HealthComponent.h"
#include "ProjectileComponent.h"

// Global function to create projectiles (to avoid circular includes)
void createProjectile(float x, float y, float dirX, float dirY, float speed) {
    Engine::createProjectile(x, y, dirX, dirY, speed);
}

// Definition of static members
bool Engine::isRunning = false;
bool Engine::gameOver = false;
bool Engine::needsRestart = false;
SDL_Window* Engine::window = nullptr;
SDL_Renderer* Engine::renderer = nullptr;
std::vector<std::unique_ptr<GameObject>> Engine::gameObjects;
int Engine::width = 0;
int Engine::height = 0;
GameObject* Engine::hero = nullptr;

// Create a projectile GameObject immediately
void Engine::createProjectile(float x, float y, float dirX, float dirY, float speed) {
    auto projectile = std::make_unique<GameObject>();
    projectile->add<SpriteComponent>("projectile");
    projectile->add<BodyComponent>(x, y);
    projectile->add<ProjectileComponent>(speed, dirX, dirY);
    gameObjects.push_back(std::move(projectile));
}

// Create an enemy GameObject at random location
void Engine::createEnemy() {
    if (!hero) return; // Can't create enemy without hero reference

    // Verify hero still exists in gameObjects
    bool heroExists = false;
    for (auto& obj : gameObjects) {
        if (obj.get() == hero) {
            heroExists = true;
            break;
        }
    }
    if (!heroExists) return;

    // Ensure we have valid dimensions
    int safeWidth = (width > 100) ? width - 64 : 736;
    int safeHeight = (height > 100) ? height - 64 : 536;

    auto enemy = std::make_unique<GameObject>();
    enemy->add<SpriteComponent>("enemy");
    enemy->add<BodyComponent>(rand() % safeWidth, rand() % safeHeight); // Random position, leave margin for sprite size
    enemy->add<FollowComponent>(*hero, 100.0f); // Follow the hero at 100 pixels/second
    enemy->add<HealthComponent>(1); // Enemy has 1 health

    gameObjects.push_back(std::move(enemy));
}