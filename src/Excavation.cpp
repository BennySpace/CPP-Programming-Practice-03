#include "Excavation.h"

Excavation::Excavation(const std::string &name, const std::string &terrain, int cost)
    : name(name), terrainType(terrain), cost(cost) {}

std::string Excavation::getName() const {
    return name;
}

std::string Excavation::getTerrainType() const {
    return terrainType;
}

int Excavation::getCost() const {
    return cost;
}