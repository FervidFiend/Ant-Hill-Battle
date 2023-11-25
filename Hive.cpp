// Hive.cpp
#include "Hive.h"
#include "Ant.h"

Hive::Hive(SDL_Point pos, int team) : Entity(pos, SDL_Point(GRID_SIZE, GRID_SIZE), { 0, 0, 128, 255 }), team(team) {
    teamColor = getTeamColor();
}

SDL_Color Hive::getTeamColor() {
    // Distribute hue values evenly in the range of 0 to 360 degrees
    float hue = (static_cast<float>(team % MAX_TEAMS) / MAX_TEAMS) * 360.0f;
    return HSVtoRGB(hue, 1.0f, 1.0f); // Full saturation (1.0) and value (1.0)
}

void Hive::step(std::vector<Ant*>& ants, double deltaTime) {
    lastActionTime += deltaTime;
    if (food >= FOOD_COST && lastActionTime > SPAWN_INTERVAL) {
        spawnAnt(ants);
        lastActionTime = 0;
    }
}

void Hive::spawnAnt(std::vector<Ant*>& ants) {
    SDL_Point center = getCenter();

    food -= FOOD_COST;
    idCounter += 1;

    Ant* newAnt = new Ant(center, team, idCounter, this);
    ants.push_back(newAnt);
}

int Hive::getPop(const std::vector<Ant*>& ants) {
    return std::count_if(ants.begin(), ants.end(), [this](const Ant* ant) {
        return ant->team == team;
        });
}

int Hive::getIdle(const std::vector<Ant*>& ants) {
    return std::count_if(ants.begin(), ants.end(), [this](const Ant* ant) {
        return ant->team == team && ant->state == IDLE;
        });
}