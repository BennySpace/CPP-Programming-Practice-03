#include "Excavation.h"

Excavation::Excavation(const std::string &n, const std::string &t, int c)
    : name(n), terrainType(t), cost(c) {}

std::string Excavation::getName() const {
    return name;
}

std::string Excavation::getTerrainType() const {
    return terrainType;
}

int Excavation::getCost() const {
    return cost;
}