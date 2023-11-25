// Hive.h
#pragma once
#include "Entity.h"

class Ant;

class Hive : public Entity {
public:
    Hive(SDL_Point pos, int team);

    int team = 0;
    SDL_Color teamColor;
    int food = STARTING_FOOD;
    int idCounter = 0;

    SDL_Color getTeamColor();
    void step(std::vector<Ant*>& ants, double deltaTime);
    void spawnAnt(std::vector<Ant*>& ants);
    int getPop(const std::vector<Ant*>& ants);
    int getIdle(const std::vector<Ant*>& ants);
};