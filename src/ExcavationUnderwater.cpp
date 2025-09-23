#include "ExcavationUnderwater.h"
#include <iostream>

ExcavationUnderwater::ExcavationUnderwater()
    : Excavation("Underwater expedition", "Ocean", 100) {}

void ExcavationUnderwater::excavate() {
    std::cout << "Excavating underwater: searching for amonites and marine fossils.\n";
}

void ExcavationUnderwater::effect() {
    std::cout << "Effect: high chance of finding ammonites, but a brush is required.\n";
}

void ExcavationUnderwater::printText() {
    std::cout << "You're tired, but you must keep working underwater.\n";
}

