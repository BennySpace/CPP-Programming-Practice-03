#ifndef EXCAVATIONUNDERWATER_H
#define EXCAVATIONUNDERWATER_H

#include "Excavation.h"

class ExcavationUnderwater : public Excavation {
public:
    ExcavationUnderwater();

    void excavate() override;
    void effect() override;
    void printText() override;
};

#endif // EXCAVATIONUNDERWATER_H