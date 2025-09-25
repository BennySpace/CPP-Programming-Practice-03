#ifndef EXCAVATIONMOON_H
#define EXCAVATIONMOON_H

#include "Excavation.h"

class ExcavationMoon : public Excavation {
public:
    ExcavationMoon();

    void excavate(Player& player, const std::string& equipment) override;
    void effect() const override;
    void printText() const override;
};

#endif // EXCAVATIONMOON_H