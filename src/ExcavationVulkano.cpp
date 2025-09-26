#include "ExcavationVulkano.h"

#include <iostream>

ExcavationVulkano::ExcavationVulkano()
    : Excavation("Volcanic expedition", "Volcano", 150) {
    std::random_device rd;
    rng.seed(rd());
}

void ExcavationVulkano::excavate(Player& player, const std::string& equipment) {
    if (!player.hasEquipment(equipment)) {
        std::cout << "You don't have a " << equipment << "!" << std::endl;
        return;
    }

    player.spendFood();

    for (auto& item : player.getInventory()) {
        if (item.type == "equipment" && item.name == equipment && !item.isBroken) {
            std::uniform_int_distribution<int> dist(1, 100);

            if (dist(rng) <= 25) {
                item.durability = std::max(0, item.durability - 10);
                std::cout << equipment << " took wear! Durability now: " << item.durability << std::endl;

                if (item.durability <= 0) {
                    item.isBroken = true;
                    std::cout << equipment << " is broken and needs repair!" << std::endl;
                }
            }

            break;
        }
    }

    std::cout << "Excavating on volcano with " << equipment << "..." << std::endl;
    std::uniform_int_distribution<int> dist(1, 100);
    int chance = dist(rng);

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

void ExcavationVulkano::effect(Player& player) {
    std::cout << "The heat is intense!" << std::endl;
    std::uniform_int_distribution<int> dist(1, 100);
    int chance = dist(rng);

    if (chance <= 25) {
        player.spendFood();
        std::cout << "The heat exhausted you! You consumed 1 extra food." << std::endl;
    }
}

void ExcavationVulkano::printText() const {
    std::cout << "The heat is unbearable, but you dig through volcanic ash!\n";
}
