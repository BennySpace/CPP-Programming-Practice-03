#ifndef EXCAVATIONMOON_H
#define EXCAVATIONMOON_H

#include "Excavation.h"

class ExcavationMoon : public Excavation {
public:
    ExcavationMoon();

    void excavate() override;
    void effect() override;
    void printText() override;
};

#endif // EXCAVATIONMOON_H