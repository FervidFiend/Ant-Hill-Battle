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
const double SPAWN_INTERVAL = 0.5;
const double FIGHTING_INTERVAL = 0.5;
const double MINING_INTERVAL = 1.0;
const int GRID_SIZE = 20; // Assuming each entity occupies a 20x20 grid space
const int MIN_DISTANCE = 40; // Minimum distance between mines and other entities
const int UNIT_SIZE = 5;
const int UNIT_VISION = 2;
const int FOOD_COST = 10;
const float UNIT_SPEED = 144.0f;
const int STARTING_FOOD = 100;
const int MAX_TEAMS = 10;

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

struct Vec2i {
public:
    int x;
    int y;

    Vec2i() : x(0), y(0) {}
    Vec2i(int x, int y) : x(x), y(y) {}
};

struct Vec2f {
public:
    float x;
    float y;

    Vec2f() : x(0.0f), y(0.0f) {}
    Vec2f(float x, float y) : x(x), y(y) {}
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