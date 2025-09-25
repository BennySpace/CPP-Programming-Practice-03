#ifndef EXCAVATIONVULKANO_H
#define EXCAVATIONVULKANO_H

#include "Excavation.h"

class ExcavationVulkano : public Excavation {
public:
    ExcavationVulkano();

    void excavate(Player& player, const std::string& equipment) override;
    void effect() const override;
    void printText() const override;
};

#endif // EXCAVATIONVULKANO_H