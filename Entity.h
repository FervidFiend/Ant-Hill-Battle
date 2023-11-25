// Entity.h
#pragma once
#include "GlobalConstants.h"

class Camera;

class Entity {
public:
    Entity(SDL_Point position, SDL_Point size, SDL_Color color);

    SDL_Point position;
    MovementAccumulator movementAccumulator;
    SDL_Point size;
    SDL_Color color;
    double lastActionTime = 0.0;

    SDL_Point getCenter();

    void move(SDL_FPoint deltaMovement);
    void moveInDirection(SDL_FPoint direction, double deltaTime);
    void moveTowards(SDL_Point target, double deltaTime);

    void draw(SDL_Renderer* renderer, Camera* camera);
    void drawTriangle(SDL_Renderer* renderer, Camera* camera, const SDL_Color& color);
    void drawSprite(SDL_Renderer* renderer, Camera* camera, SDL_Texture* sprite);
    void drawText(SDL_Renderer* renderer, Camera* camera, TTF_Font* font, const std::string& text, int offsetX, int offsetY);
    void drawBar(SDL_Renderer* renderer, Camera* camera, int current, int max, int barWidth, int barHeight, int offsetX, int offsetY, SDL_Color barColor, SDL_Color bgColor);
};