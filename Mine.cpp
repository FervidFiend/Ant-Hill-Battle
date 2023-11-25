// Mine.cpp
#include "Mine.h"
#include "Ant.h"

Mine::Mine(SDL_Point pos) : Entity(pos, SDL_Point(GRID_SIZE, GRID_SIZE), { 0, 128, 0, 255 }) {

}

bool Mine::checkVacancy() {
	if (occupants.size() >= maxOccupants) {
		return false;
	}
	return true;
}

bool Mine::hasMiner(Ant* ant) {
	return std::find(occupants.begin(), occupants.end(), ant) != occupants.end();
}

void Mine::addMiner(Ant* ant) {
	if (checkVacancy()) {
		occupants.push_back(ant);
	}
}

void Mine::removeMiner(Ant* ant) {
	auto it = std::find(occupants.begin(), occupants.end(), ant);
	if (it != occupants.end()) {
		occupants.erase(it);
	}
}

void Mine::step() {
	SDL_Point thisCenter = getCenter();
	for (Ant* ant : occupants) {
		SDL_Point otherCenter = ant->getCenter(); // Center of the mine

		float dx = otherCenter.x - thisCenter.x;
		float dy = otherCenter.y - thisCenter.y;
		float distance = sqrt(dx * dx + dy * dy); // Euclidean distance
		if (distance != 0 || ant->hp <= 0) {
			removeMiner(ant);
		}
	}
}