#ifndef EXCAVATION_H
#define EXCAVATION_H

#include "Player.h"
#include <string>
#include <random>

class Excavation {
protected:
    std::string name;
    std::string terrainType;
    int cost;
    std::mt19937 rng;

public:
    Excavation(const std::string& name, const std::string& terrain, int cost);
    virtual ~Excavation() = default;

    virtual void excavate(Player& player, const std::string& equipment) = 0;
    virtual void effect(Player& player) = 0;
    virtual void printText() const = 0;

    std::string getName() const;
    std::string getTerrainType() const;
    int getCost() const;
};

#endif // EXCAVATION_H