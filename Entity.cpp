// Entity.cpp
#include "Entity.h"
#include "Camera.h"

Entity::Entity(SDL_Point position, SDL_Point size, SDL_Color color) : position(position), size(size), color(color) {

}


SDL_Point Entity::getCenter() {
    SDL_Point center;
    // Calculate the center of the entity
    center.x = position.x + (size.x / 2);
    center.y = position.y + (size.y / 2);
    return center;
}


void Entity::move(SDL_FPoint deltaMovement) {
    SDL_Point move = movementAccumulator.addAndGetMovement(deltaMovement);

    position.x += move.x;
    position.y += move.y;
}

void Entity::moveInDirection(SDL_FPoint direction, double deltaTime) {
    SDL_FPoint deltaMovement = { direction.x * (UNIT_SPEED * deltaTime), direction.y * (UNIT_SPEED * deltaTime) };
    move(deltaMovement);
}

void Entity::moveTowards(SDL_Point target, double deltaTime) {
    SDL_FPoint direction;
    direction.x = target.x - getCenter().x;
    direction.y = target.y - getCenter().y;

    float magnitude = sqrt(direction.x * direction.x + direction.y * direction.y);
    if (magnitude != 0.0f) {
        direction.x /= magnitude;
        direction.y /= magnitude;
    }
    else {
        direction = SDL_FPoint(0.0f, 0.0f);
    }

    moveInDirection(direction, deltaTime);
}


void Entity::draw(SDL_Renderer* renderer, Camera* camera) {
    SDL_Point screenPos = camera->convertWorldToScreenCoords({ static_cast<int>(position.x), static_cast<int>(position.y) });

    SDL_Rect rect = {
        screenPos.x,
        screenPos.y,
        static_cast<int>(size.x * camera->zoom),
        static_cast<int>(size.y * camera->zoom)
    };

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

void Entity::drawTriangle(SDL_Renderer* renderer, Camera* camera, const SDL_Color& color) {
    // Convert the triangle's points from world to screen coordinates
    SDL_Point screenBottomLeft = camera->convertWorldToScreenCoords({ static_cast<int>(position.x), static_cast<int>(position.y + size.y) });
    SDL_Point screenMidLeft = camera->convertWorldToScreenCoords({ static_cast<int>(position.x), static_cast<int>(position.y + size.y * 0.25f) });
    SDL_Point screenBottomMid = camera->convertWorldToScreenCoords({ static_cast<int>(position.x + size.x * 0.75f), static_cast<int>(position.y + size.y) });

    SDL_FPoint bottomLeft = { static_cast<float>(screenBottomLeft.x), static_cast<float>(screenBottomLeft.y) };
    SDL_FPoint midLeft = { static_cast<float>(screenMidLeft.x), static_cast<float>(screenMidLeft.y) };
    SDL_FPoint bottomMid = { static_cast<float>(screenBottomMid.x), static_cast<float>(screenBottomMid.y) };

    std::vector<SDL_Vertex> verts = {
        { bottomLeft, color, {0, 0} },
        { midLeft, color, {0, 0} },
        { bottomMid, color, {0, 0} }
    };

    SDL_RenderGeometry(renderer, nullptr, verts.data(), verts.size(), nullptr, 0);
}

void Entity::drawSprite(SDL_Renderer* renderer, Camera* camera, SDL_Texture* sprite) {
    if (sprite == nullptr) {
        throw std::runtime_error("Sprite is not loaded!");
    }

    // Convert the sprite's position from world to screen coordinates
    SDL_Point screenPos = camera->convertWorldToScreenCoords({ static_cast<int>(position.x), static_cast<int>(position.y) });

    // Create a rectangle for rendering the sprite, scaled by the camera zoom
    SDL_Rect renderQuad = {
        screenPos.x,
        screenPos.y,
        static_cast<int>(size.x * camera->zoom),
        static_cast<int>(size.y * camera->zoom)
    };

    // Render the sprite to the screen
    SDL_RenderCopy(renderer, sprite, nullptr, &renderQuad);
}

void Entity::drawText(SDL_Renderer* renderer, Camera* camera, TTF_Font* font, const std::string& text, int offsetX, int offsetY) {
    if (text == "") {
        throw std::runtime_error("Text Cannot Be Empty");
    }

    // Set text color
    SDL_Color textColor = { 255, 255, 255, 255 }; // White color

    // Create a surface from the text
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    // Convert the text's position from world to screen coordinates
    SDL_Point screenPos = camera->convertWorldToScreenCoords({
        static_cast<int>(position.x + ((size.x / 2) - (surface->w / 2) + offsetX)),
        static_cast<int>(position.y + ((size.y / 2) - (surface->h / 2) + offsetY))
        });

    SDL_Rect textRect = {
        screenPos.x,
        screenPos.y,
        static_cast<int>(surface->w * camera->zoom),
        static_cast<int>(surface->h * camera->zoom)
    };

    // Render the text
    SDL_RenderCopy(renderer, texture, NULL, &textRect);

    // Clean up
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void Entity::drawBar(SDL_Renderer* renderer, Camera* camera, int current, int max, int barWidth, int barHeight, int offsetX, int offsetY, SDL_Color barColor, SDL_Color bgColor) {
    if (max <= 0) {
        throw std::runtime_error("Max value must be greater than 0");
    }
    if (current < 0) {
        current = 0;  // Clamp the current value to 0
    }
    if (current > max) {
        current = max;  // Clamp the current value to max
    }

    // Convert the bar's position from world to screen coordinates
    SDL_Point screenPos = camera->convertWorldToScreenCoords({ position.x + offsetX, position.y + offsetY });

    // Scale the width of the filled part of the bar and the bar itself by the camera zoom
    int scaledBarWidth = static_cast<int>(barWidth * camera->zoom);
    int scaledBarHeight = static_cast<int>(barHeight * camera->zoom);
    int filledWidth = static_cast<int>((static_cast<double>(current) / max) * scaledBarWidth);

    // Draw the background of the bar
    SDL_Rect bgRect = { screenPos.x, screenPos.y, scaledBarWidth, scaledBarHeight };
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(renderer, &bgRect);

    // Draw the filled part of the bar
    SDL_Rect filledRect = { screenPos.x, screenPos.y, filledWidth, scaledBarHeight };
    SDL_SetRenderDrawColor(renderer, barColor.r, barColor.g, barColor.b, barColor.a);
    SDL_RenderFillRect(renderer, &filledRect);
}
