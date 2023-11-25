// Camera.h
#pragma once
#include "Entity.h"

struct Camera : public Entity {
    Camera();

    SDL_Rect view;  // The viewport in screen coordinates
    MovementAccumulator movementAccumulator;
    float zoom;

    SDL_Point convertWorldToScreenCoords(const SDL_Point& worldPos);
    SDL_Point screenToWorldCoords(const SDL_Point& screenPos);
};