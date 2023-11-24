// Ant.h
#pragma once
#include "Entity.h"

class Mine;
class Hive;

class Ant : public Entity {
public:
    Ant(Vec2i pos, int team, int id, Hive* homeBase);

    int team = 0;
    int id = 0;
    int hp = 0;

    antStates state;
    antTypes type;

    std::vector<Mine*> validMines;
    std::vector<Ant*> validEnemys;
    Hive* homeBase;

    int maxOpponents = 3;
    std::vector<Ant*> opponents;

    bool checkVacancy();
    bool hasOpponent(Ant* ant);
    void addOpponent(Ant* ant);
    void removeOpponent(Ant* ant);

    void step(std::vector<Mine*>& mines, std::vector<Ant*>& ants, double deltaTime);
};