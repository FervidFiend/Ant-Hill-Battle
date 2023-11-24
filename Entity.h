// Entity.h
#pragma once
#include "GlobalConstants.h"

class Entity {
public:
    Entity(Vec2i position, Vec2i size, SDL_Color color);

    Vec2i position;
    Vec2f accumulatedMovement;
    Vec2i size;
    SDL_Color color;
    double lastActionTime = 0.0;

    Vec2i getCenter();

    void moveTowards(Vec2i target, double deltaTime);
    void moveInDirection(Vec2f direction, double deltaTime);

    void draw(SDL_Renderer* renderer);
    void drawTriangle(SDL_Renderer* renderer, const SDL_Color& color);
    void drawSprite(SDL_Renderer* renderer, SDL_Texture* sprite);
    void drawText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int offsetX, int offsetY);
    void drawBar(SDL_Renderer* renderer, int current, int max, int barWidth, int barHeight, int offsetX, int offsetY, SDL_Color barColor, SDL_Color bgColor);
};