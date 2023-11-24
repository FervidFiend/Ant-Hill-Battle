// Mine.cpp
#include "Mine.h"
#include "Ant.h"

Mine::Mine(Vec2i pos) : Entity(pos, Vec2i(GRID_SIZE, GRID_SIZE), { 0, 128, 0, 255 }) {

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
	Vec2i thisCenter = getCenter();
	for (Ant* ant : occupants) {
		Vec2i otherCenter = ant->getCenter(); // Center of the mine

		float dx = otherCenter.x - thisCenter.x;
		float dy = otherCenter.y - thisCenter.y;
		float distance = sqrt(dx * dx + dy * dy); // Euclidean distance
		if (distance != 0 || ant->hp <= 0) {
			removeMiner(ant);
		}
	}
}