// Entity.cpp
#include "Entity.h"

Entity::Entity(Vec2i position, Vec2i size, SDL_Color color) : position(position), size(size), color(color) {

}


Vec2i Entity::getCenter() {
    Vec2i center;
    // Calculate the center of the entity
    center.x = position.x + (size.x / 2);
    center.y = position.y + (size.y / 2);
    return center;
}


void Entity::moveTowards(Vec2i target, double deltaTime) {
    Vec2f direction;
    direction.x = target.x - getCenter().x;
    direction.y = target.y - getCenter().y;

    float magnitude = sqrt(direction.x * direction.x + direction.y * direction.y);
    if (magnitude != 0.0f) {
        direction.x /= magnitude;
        direction.y /= magnitude;
    }
    else {
        direction = Vec2f(0.0f, 0.0f);
    }

    moveInDirection(direction, deltaTime);
}

void Entity::moveInDirection(Vec2f direction, double deltaTime) {
    accumulatedMovement.x += direction.x * (UNIT_SPEED * deltaTime);
    accumulatedMovement.y += direction.y * (UNIT_SPEED * deltaTime);

    // Update the integer coordinates only when the accumulated movement exceeds 1.0
    int moveX = static_cast<int>(accumulatedMovement.x);
    int moveY = static_cast<int>(accumulatedMovement.y);

    position.x += moveX;
    position.y += moveY;

    // Subtract the integer movement from the accumulated movement to retain the fraction
    accumulatedMovement.x -= moveX;
    accumulatedMovement.y -= moveY;
}


void Entity::draw(SDL_Renderer* renderer) {
    SDL_Rect rect = { static_cast<int>(position.x), static_cast<int>(position.y),
                      static_cast<int>(size.x), static_cast<int>(size.y) };
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

void Entity::drawTriangle(SDL_Renderer* renderer, const SDL_Color& color) {
    // Calculate points for the triangle to cover the bottom left 25% of the entity
    SDL_FPoint bottomLeft = { position.x, position.y + size.y };
    SDL_FPoint midLeft = { position.x, position.y + size.y * 0.25f };
    SDL_FPoint bottomMid = { position.x + size.x * 0.75f, position.y + size.y };

    std::vector<SDL_Vertex> verts = {
        { bottomLeft, color, {0, 0} },
        { midLeft, color, {0, 0} },
        { bottomMid, color, {0, 0} }
    };

    SDL_RenderGeometry(renderer, nullptr, verts.data(), verts.size(), nullptr, 0);
}

void Entity::drawSprite(SDL_Renderer* renderer, SDL_Texture* sprite) {
    if (sprite == nullptr) {
        throw std::runtime_error("Sprite is not loaded!");
    }

    // Create a rectangle for rendering the sprite
    SDL_Rect renderQuad = { static_cast<int>(position.x), static_cast<int>(position.y),
                            static_cast<int>(size.x), static_cast<int>(size.y) };

    // Render the sprite to the screen
    SDL_RenderCopy(renderer, sprite, nullptr, &renderQuad);
}

void Entity::drawText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int offsetX, int offsetY) {
    if (text == "") {
        throw std::runtime_error("Text Cannot Be Empty");
    }
    // Set text color
    SDL_Color textColor = { 255, 255, 255, 255 }; // White color

    // Create a surface from the text
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    // Calculate position to draw the text above the entity
    int textX = position.x + ((size.x / 2) - (surface->w / 2) + offsetX);
    int textY = position.y + ((size.y / 2) - (surface->h / 2) + offsetY);

    SDL_Rect textRect = { textX, textY, surface->w, surface->h };

    // Render the text
    SDL_RenderCopy(renderer, texture, NULL, &textRect);

    // Clean up
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void Entity::drawBar(SDL_Renderer* renderer, int current, int max, int barWidth, int barHeight, int offsetX, int offsetY, SDL_Color barColor, SDL_Color bgColor) {
    if (max <= 0) {
        throw std::runtime_error("Max value must be greater than 0");
    }
    if (current < 0) {
        current = 0;  // Clamp the current value to 0
    }
    if (current > max) {
        current = max;  // Clamp the current value to max
    }

    // Calculate the width of the filled part of the bar
    int filledWidth = static_cast<int>((static_cast<double>(current) / max) * barWidth);

    // Calculate the position of the bar
    int barX = position.x + offsetX;
    int barY = position.y + offsetY;

    // Draw the background of the bar
    SDL_Rect bgRect = { barX, barY, barWidth, barHeight };
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(renderer, &bgRect);

    // Draw the filled part of the bar
    SDL_Rect filledRect = { barX, barY, filledWidth, barHeight };
    SDL_SetRenderDrawColor(renderer, barColor.r, barColor.g, barColor.b, barColor.a);
    SDL_RenderFillRect(renderer, &filledRect);
}
