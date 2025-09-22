#ifndef EXCAVATION_H
#define EXCAVATION_H

#include <string>

class Excavation {
public:
    Excavation(const std::string& n, const std::string& t, int c);
    virtual ~Excavation() = default;

    virtual void excavate() = 0;
    virtual void effect() = 0;
    virtual void printText() = 0;

    std::string getName() const;
    std::string getTerrainType() const;
    int getCost() const;

protected:
    std::string name;
    std::string terrainType;
    int cost;
};

#endif // EXCAVATION_H