#ifndef EXCAVATIONVULKANO_H
#define EXCAVATIONVULKANO_H

#include "Excavation.h"

class ExcavationVulkano : public Excavation {
public:
    ExcavationVulkano();

    void excavate() override;
    void effect() override;;
    void printText();
};

#endif // EXCAVATIONVULKANO_H