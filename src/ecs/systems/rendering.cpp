/*
 * rendering.cpp
 */

#include "rendering.h"
#include "../../game.h"

void RenderingSystem::Update() {
    SDL_SetRenderDrawColor(game::renderer, 255, 255, 255, 255);
    SDL_RenderClear(game::renderer);

    for (Entity entity : world::entity_manager->GetEntitiesWithComponent<Sprite>()) {
        if(world::entity_manager->GetComponent<Transform2D>(entity)) {
            Draw(entity);
        }
    }

    SDL_RenderPresent(game::renderer);
}

void RenderingSystem::Draw(Entity entity) {
    Image* image = world::entity_manager->GetComponent<Sprite>(entity)->image;
    Transform2D* transform = world::entity_manager->GetComponent<Transform2D>(entity);

    Vector2 position = transform->position - world::entity_manager->GetComponent<Transform2D>(world::camera)->position - Vector2{-WINDOW_WIDTH/2,-WINDOW_HEIGHT/2};
    Vector2 size = transform->size;

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

