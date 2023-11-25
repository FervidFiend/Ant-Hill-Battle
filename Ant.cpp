// Ant.cpp
#include "Ant.h"
#include "Mine.h"
#include "Hive.h"

Ant::Ant(SDL_Point pos, int team, int id, Hive* homeBase) : Entity(pos, SDL_Point(UNIT_SIZE, UNIT_SIZE), homeBase->teamColor), team(team), id(id), hp(3), homeBase(homeBase) {
    type = antTypes(rand() % 2);
    if (type == FIGHTER) {
        hp = 5;
    }
}

bool Ant::checkVacancy() {
    if (opponents.size() >= maxOpponents) {
        return false;
    }
    return true;
}

bool Ant::hasOpponent(Ant* ant) {
    return std::find(opponents.begin(), opponents.end(), ant) != opponents.end();
}

void Ant::addOpponent(Ant* ant) {
    if (checkVacancy()) {
        opponents.push_back(ant);
    }
}

void Ant::removeOpponent(Ant* ant) {
    auto it = std::find(opponents.begin(), opponents.end(), ant);
    if (it != opponents.end()) {
        opponents.erase(it);
    }
}

void Ant::step(std::vector<Mine*>& mines, std::vector<Ant*>& ants, double deltaTime) {
    float thresholdSquared = std::pow(UNIT_SPEED * deltaTime, 2);
    lastActionTime += deltaTime;
    SDL_Point thisCenter = getCenter(); // Assuming this returns the center of the current object
    validMines.clear();
    validEnemys.clear();

    for (Ant* ant : opponents) {
        SDL_Point otherCenter = ant->getCenter(); // Center of the mine

        float dx = otherCenter.x - thisCenter.x;
        float dy = otherCenter.y - thisCenter.y;
        float distance = sqrt(dx * dx + dy * dy); // Euclidean distance
        if (distance > UNIT_SIZE * UNIT_SIZE) {
            removeOpponent(ant);
        }
    }

    for (Ant* ant : ants) {
        if (ant->team != team) {
            SDL_Point otherCenter = ant->getCenter(); // Center of the mine

            float dx = otherCenter.x - thisCenter.x;
            float dy = otherCenter.y - thisCenter.y;
            float distance = sqrt(dx * dx + dy * dy); // Euclidean distance

            if (distance < MIN_DISTANCE * UNIT_VISION * (type == FIGHTER ? 4 : 1)) {
                if (ant->hasOpponent(this) || ant->checkVacancy()) {
                    validEnemys.push_back(ant);
                }
            }
        }
    }
    for (Mine* mine : mines) {
        SDL_Point otherCenter = mine->getCenter(); // Center of the mine

        float dx = otherCenter.x - thisCenter.x;
        float dy = otherCenter.y - thisCenter.y;
        float distance = sqrt(dx * dx + dy * dy); // Euclidean distance

        if (distance < MIN_DISTANCE * UNIT_VISION * 2) {
            if (type == MINER && (mine->hasMiner(this) || mine->checkVacancy())) {
                validMines.push_back(mine);
            }
        }
    }



    if (!validEnemys.empty()) {
        Ant* closestEnemy = validEnemys[0];
        float minDistanceSquared = std::numeric_limits<float>::max();

        // Find the closest Enemy
        for (Ant* ant : validEnemys) {
            SDL_Point diff;
            diff.x = ant->getCenter().x - thisCenter.x;
            diff.y = ant->getCenter().y - thisCenter.y;

            float distanceSquared = diff.x * diff.x + diff.y * diff.y;
            if (distanceSquared < minDistanceSquared) {
                minDistanceSquared = distanceSquared;
                closestEnemy = ant;
            }
        }

        if (minDistanceSquared < UNIT_SIZE * UNIT_SIZE) {
            // At the closest Enemy
            if (closestEnemy->hasOpponent(this)) {
                if (lastActionTime > MINING_INTERVAL) {
                    closestEnemy->hp -= 1;
                    lastActionTime = 0;
                }
            }
            else {
                closestEnemy->addOpponent(this);
            }
            state = FIGHTING;
        }
        else if (minDistanceSquared < thresholdSquared) {
            SDL_Point enemyCenter = closestEnemy->getCenter();
            this->position.x = enemyCenter.x - this->size.x / 2;
            this->position.y = enemyCenter.y - this->size.y / 2;
            state = MOVINGTOENEMY;
        }
        else {
            // Move towards the closest Enemy
            moveTowards(closestEnemy->getCenter(), deltaTime); 
            state = MOVINGTOENEMY;
        }
    }
    else if (!validMines.empty()) {
        Mine* closestMine = validMines[0];
        float minDistanceSquared = std::numeric_limits<float>::max();

        // Find the closest mine
        for (Mine* mine : validMines) {
            SDL_Point diff;
            diff.x = mine->getCenter().x - thisCenter.x;
            diff.y = mine->getCenter().y - thisCenter.y;

            float distanceSquared = diff.x * diff.x + diff.y * diff.y;
            if (distanceSquared < minDistanceSquared) {
                minDistanceSquared = distanceSquared;
                closestMine = mine;
            }
        }

        if (minDistanceSquared == 0) {
            // At the closest mine
            if (closestMine->hasMiner(this)) {
                if (lastActionTime > MINING_INTERVAL) {
                    homeBase->food += 1;
                    lastActionTime = 0;
                }
            }
            else {
                closestMine->addMiner(this);
            }
            state = MINING;
        }
        else if (minDistanceSquared < thresholdSquared) {
            SDL_Point mineCenter = closestMine->getCenter();
            this->position.x = mineCenter.x - this->size.x / 2;
            this->position.y = mineCenter.y - this->size.y / 2;
            state = MOVINGTOMINE;
        }
        else {
            // Move towards the closest mine
            moveTowards(closestMine->getCenter(), deltaTime);
            state = MOVINGTOMINE;
        }
    }
    else {
        SDL_FPoint direction((rand() % 3) - 1, (rand() % 3) - 1);
        moveInDirection(direction, deltaTime);
        state = IDLE;
    }

    position.x = std::min(std::max(0, position.x), WORLD_WIDTH - UNIT_SIZE);
    position.y = std::min(std::max(0, position.y), WORLD_HEIGHT - UNIT_SIZE);
}