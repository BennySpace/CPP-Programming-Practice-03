#include "ExcavationVulkano.h"

#include <iostream>

ExcavationVulkano::ExcavationVulkano()
    : Excavation("Volcanic expedition", "Volcano", 150) {
}

void ExcavationVulkano::excavate() {
    std::cout << "Excavating on a volcano: searching for dinosaur fossils.\n";
}

void ExcavationVulkano::effect() {
    std::cout << "Effect: chance to find rare bones, but a pickaxe may damage loot.\n";
}

void ExcavationVulkano::printText() {
    std::cout << "The heat is unbearable, but you dig through volcanic ash!\n";
}
