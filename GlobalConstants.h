// GlobalConstants.h
#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;
const int WORLD_WIDTH = 4000;
const int WORLD_HEIGHT = 500;
const double SPAWN_INTERVAL = 0.05;
const double FIGHTING_INTERVAL = 0.5;
const double MINING_INTERVAL = 1.0;
const int GRID_SIZE = 20; // Assuming each entity occupies a 20x20 grid space
const int MIN_DISTANCE = 40; // Minimum distance between mines and other entities
const int UNIT_SIZE = 5;
const int UNIT_VISION = 2;
const int FOOD_COST = 10;
const float UNIT_SPEED = 144.0f;
const int STARTING_FOOD = 100;
const int MAX_TEAMS = 100;

enum antStates {
    MOVINGTOENEMY,
    FIGHTING,
    MOVINGTOMINE,
    MINING,
    IDLE
};
enum antTypes {
    MINER,
    FIGHTER
};

static SDL_Color HSVtoRGB(float h, float s, float v) {
    float r = 0, g = 0, b = 0;

    int i = static_cast<int>(h / 60.0f) % 6;
    float f = h / 60.0f - static_cast<float>(i);
    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));

    switch (i) {
    case 0: r = v, g = t, b = p; break;
    case 1: r = q, g = v, b = p; break;
    case 2: r = p, g = v, b = t; break;
    case 3: r = p, g = q, b = v; break;
    case 4: r = t, g = p, b = v; break;
    case 5: r = v, g = p, b = q; break;
    }

    return { static_cast<Uint8>(r * 255), static_cast<Uint8>(g * 255), static_cast<Uint8>(b * 255), 255 };
}

struct MovementAccumulator {
    SDL_FPoint accumulatedMovement;

    MovementAccumulator() : accumulatedMovement{ 0.0f, 0.0f } {}

    SDL_Point addAndGetMovement(const SDL_FPoint& deltaMovement) {
        // Add the delta movement to the accumulated movement
        accumulatedMovement.x += deltaMovement.x;
        accumulatedMovement.y += deltaMovement.y;

        // Extract the integer part of the accumulated movement
        SDL_Point intMovement = {
            static_cast<int>(accumulatedMovement.x),
            static_cast<int>(accumulatedMovement.y)
        };

        // Subtract the integer part from the accumulated movement
        accumulatedMovement.x -= intMovement.x;
        accumulatedMovement.y -= intMovement.y;

        return intMovement;
    }
};