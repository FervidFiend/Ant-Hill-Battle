// Mine.h
#pragma once
#include "Entity.h"

class Ant;

class Mine : public Entity {
public:
    Mine(SDL_Point pos);

    int maxOccupants = 3;
    std::vector<Ant*> occupants;

    bool checkVacancy();
    bool hasMiner(Ant* ant);
    void addMiner(Ant* ant);
    void removeMiner(Ant* ant);

    void step();
};