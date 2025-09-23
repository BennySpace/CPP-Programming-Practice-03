#include "ExcavationMoon.h"
#include <iostream>

#include "ExcavationVulkano.h"

ExcavationMoon::ExcavationMoon()
    : Excavation("Lunar expedition", "Moon", 200) {}

void ExcavationMoon::excavate() {
    std::cout << "Excavating on the Moon: searching for cosmic fossils.\n";
}

void ExcavationMoon::effect() {
    std::cout << "Effect: unique loot, but special equipment is required.\n";
}

void ExcavationMoon::printText() {
    std::cout << "Low gravity, but digging is still tough!\n";
}
