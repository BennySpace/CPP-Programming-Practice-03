#ifndef EXCAVATION_H
#define EXCAVATION_H

#include "Player.h"
#include <string>

class Excavation {
protected:
    std::string name;
    std::string terrainType;
    int cost;

public:
    Excavation(const std::string& n, const std::string& t, int c);
    virtual ~Excavation() = default;

    virtual void excavate(Player& player, const std::string& equipment) = 0;
    virtual void effect() = 0;
    virtual void printText() = 0;

    std::string getName() const;
    std::string getTerrainType() const;
    int getCost() const;
};

#endif // EXCAVATION_H