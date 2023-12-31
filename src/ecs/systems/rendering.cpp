/*
 * rendering.cpp
 */

#include "rendering.h"
#include "../../game.h"
#include <math.h>

void RenderingSystem::Update() {
    SDL_SetRenderDrawColor(game::renderer, 255, 255, 255, 255);
    SDL_RenderClear(game::renderer);

    SparseSet<Entity>& spriteEntities = world::entity_manager->GetEntitiesWithComponent<Sprite>();
    SparseSet<Entity>& transformEntities = world::entity_manager->GetEntitiesWithComponent<Transform2D>();

    auto spriteIt = spriteEntities.begin();
    auto transformIt = transformEntities.begin();

    while (spriteIt != spriteEntities.end() && transformIt != transformEntities.end()) {
        Entity spriteEntity = *spriteIt;
        Entity transformEntity = *transformIt;

        if (spriteEntity == transformEntity) {
            Sprite* sprite = world::entity_manager->GetComponent<Sprite>(spriteEntity);
            Transform2D* transform = world::entity_manager->GetComponent<Transform2D>(transformEntity);

            Draw(sprite, transform);

            ++spriteIt;
            ++transformIt;
        }
        else if (spriteEntity < transformEntity) {
            ++spriteIt;
        }
        else {
            ++transformIt;
        }
    }

    SDL_RenderPresent(game::renderer);
}

void RenderingSystem::Draw(Sprite* sprite, Transform2D* transform) {
    Image* image = sprite->image;
    Transform2D* camera = world::entity_manager->GetComponent<Transform2D>(world::camera);

    if (transform->position.x + transform->size.x/2 <= camera->position.x - camera->size.x/2 ||
        transform->position.x - transform->size.x/2 >= camera->position.x + camera->size.x/2 ||
        transform->position.y + transform->size.y/2 <= camera->position.y - camera->size.y/2 ||
        transform->position.y - transform->size.y/2 >= camera->position.y + camera->size.y/2) {
        return;
    }

    Vector2 scale = Vector2{WINDOW_WIDTH/camera->size.x, WINDOW_HEIGHT/camera->size.y};
    Vector2 position = scale * (transform->position - Vector2{round(camera->position.x),round(camera->position.y)}) - Vector2{-WINDOW_WIDTH/2,-WINDOW_HEIGHT/2};
    position.x = round(position.x);
    position.y = round(position.y);
    Vector2 size = transform->size * scale;

    // Calculate the centered position
    int centerX = position.x - static_cast<int>(size.x / 2);
    int centerY = position.y - static_cast<int>(size.y / 2);

    // Calculate the scaled destination rectangle
    SDL_Rect scaledRect;
    scaledRect.x = centerX;
    scaledRect.y = centerY;
    scaledRect.w = static_cast<int>(size.x);
    scaledRect.h = static_cast<int>(size.y);

    SDL_RenderCopy(image->renderer, image->texture, &(image->srect), &scaledRect);
}

