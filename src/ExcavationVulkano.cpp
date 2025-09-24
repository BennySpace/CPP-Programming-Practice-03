#include "ExcavationVulkano.h"

#include <iostream>

ExcavationVulkano::ExcavationVulkano()
    : Excavation("Volcanic expedition", "Volcano", 150) {
}

void ExcavationVulkano::excavate(Player& player, const std::string& equipment) {
    if (!player.hasEquipment(equipment)) {
        std::cout << "You don't have a " << equipment << "!" << std::endl;
        return;
    }

    player.spendFood();
    std::cout << "Excavating on volcano with " << equipment << "..." << std::endl;
    int chance = std::rand() % 100;

    if (equipment == "Shovel" && chance < 80) {
        player.addItem(Item("Dinosaur Bone", "loot", 100));
        std::cout << "Found a Dinosaur Bone!" << std::endl;
    } else if (equipment == "Brush" && chance < 40) {
        player.addItem(Item("Fossil Fragment", "loot", 20));
        std::cout << "Found a Fossil Fragment!" << std::endl;
    } else if (equipment == "Pickaxe" && chance < 10) {
        std::cout << "Used Pickaxe, but the loot was damaged!" << std::endl;
    } else {
        std::cout << "No loot found this time." << std::endl;
    }
}

void ExcavationVulkano::effect() {
    std::cout << "Effect: chance to find rare bones, but a pickaxe may damage loot.\n";
}

void ExcavationVulkano::printText() {
    std::cout << "The heat is unbearable, but you dig through volcanic ash!\n";
}
