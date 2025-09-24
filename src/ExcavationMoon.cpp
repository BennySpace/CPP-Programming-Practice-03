#include "ExcavationMoon.h"
#include <iostream>

#include "ExcavationVulkano.h"

ExcavationMoon::ExcavationMoon()
    : Excavation("Lunar expedition", "Moon", 200) {}

void ExcavationMoon::excavate(Player& player, const std::string& equipment) {
    if (!player.hasEquipment(equipment)) {
        std::cout << "You don't have a " << equipment << "!" << std::endl;
        return;
    }

    player.spendFood();
    std::cout << "Excavating on the Moon with " << equipment << "..." << std::endl;
    int chance = std::rand() % 100;

    if (equipment == "Pickaxe" && chance < 60) {
        player.addItem(Item("Cosmic Fossil", "loot", 150));
        std::cout << "Found a Cosmic Fossil!" << std::endl;
    } else if (equipment == "Brush" && chance < 30) {
        player.addItem(Item("Lunar Rock", "loot", 30));
        std::cout << "Found a Lunar Rock!" << std::endl;
    } else if (equipment == "Shovel" && chance < 15) {
        std::cout << "Used Shovel, but the loot was damaged!" << std::endl;
    } else {
        std::cout << "No loot found this time." << std::endl;
    }
}

void ExcavationMoon::effect() {
    std::cout << "Effect: unique loot, but special equipment is required.\n";
}

void ExcavationMoon::printText() {
    std::cout << "Low gravity, but digging is still tough!\n";
}
