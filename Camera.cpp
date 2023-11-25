// Camera.cpp
#include "Camera.h"

Camera::Camera()
    : Entity({ 0, 0 }, SDL_Point(GRID_SIZE, GRID_SIZE), { 255, 255, 255, 255 }), view{ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT }, zoom(1.0f) {}

SDL_Point Camera::convertWorldToScreenCoords(const SDL_Point& worldPos) {
    SDL_Point screenPos;

    // Calculate the scaled viewport size
    int scaledWidth = static_cast<int>(view.w / zoom);
    int scaledHeight = static_cast<int>(view.h / zoom);

    // Calculate the top-left corner of the scaled viewport in the world
    int cornerX = position.x - scaledWidth / 2;
    int cornerY = position.y - scaledHeight / 2;

    // Convert world coordinates to screen coordinates, considering the zoom level
    screenPos.x = static_cast<int>((worldPos.x - cornerX) * zoom);
    screenPos.y = static_cast<int>((worldPos.y - cornerY) * zoom);

    return screenPos;
}

SDL_Point Camera::screenToWorldCoords(const SDL_Point& screenPos) {
    SDL_Point worldPos;

    // Calculate the scaled viewport size
    int scaledWidth = static_cast<int>(view.w / zoom);
    int scaledHeight = static_cast<int>(view.h / zoom);

    // Calculate the top-left corner of the scaled viewport in the world
    int cornerX = position.x - scaledWidth / 2;
    int cornerY = position.y - scaledHeight / 2;

    // Convert screen coordinates to world coordinates, considering the zoom level
    worldPos.x = static_cast<int>((screenPos.x / zoom) + cornerX);
    worldPos.y = static_cast<int>((screenPos.y / zoom) + cornerY);

    return worldPos;
}
