#ifndef EXCAVATIONUNDERWATER_H
#define EXCAVATIONUNDERWATER_H

#include "Excavation.h"

class ExcavationUnderwater : public Excavation {
public:
    ExcavationUnderwater();

    void excavate(Player& player, const std::string& equipment) override;
    void effect(Player& player) override;
    void printText() const override;
};

#endif // EXCAVATIONUNDERWATER_H