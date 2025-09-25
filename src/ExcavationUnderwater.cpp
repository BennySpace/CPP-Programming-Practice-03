#include "ExcavationUnderwater.h"
#include <iostream>

ExcavationUnderwater::ExcavationUnderwater() : Excavation("Underwater expedition", "Ocean", 100) {
    std::random_device rd;
    rng.seed(rd());
}

void ExcavationUnderwater::excavate(Player& player, const std::string& equipment) {
    if (!player.hasEquipment(equipment)) {
        std::cout << "You don't have a " << equipment << "!" << std::endl;
        return;
    }

    player.spendFood();
    std::cout << "Excavating underwater with " << equipment << "..." << std::endl;
    std::uniform_int_distribution<int> dist(1, 100);
    int chance = dist(rng);

    if (equipment == "Brush" && chance < 80) {
        player.addItem(Item("Ammonite", "loot", 50));
        std::cout << "Found an Ammonite!" << std::endl;
    } else if (equipment == "Shovel" && chance < 50) {
        player.addItem(Item("Marine Fossil", "loot", 30));
        std::cout << "Found a Marine Fossil!" << std::endl;
    } else if (equipment == "Pickaxe" && chance < 50) {
        std::cout << "Used Pickaxe, but the loot was damaged!" << std::endl;
    } else {
        std::cout << "No loot found this time." << std::endl;
    }
}

void ExcavationUnderwater::effect() const {
    std::cout << "Effect: high chance of finding ammonites, but a brush is required.\n";
}

void ExcavationUnderwater::printText() const {
    std::cout << "You're tired, but you must keep working underwater.\n";
}