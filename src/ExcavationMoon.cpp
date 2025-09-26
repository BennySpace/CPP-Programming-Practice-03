#include "ExcavationMoon.h"
#include <iostream>

#include "ExcavationVulkano.h"

ExcavationMoon::ExcavationMoon()
    : Excavation("Lunar expedition", "Moon", 200) {
    std::random_device rd;
    rng.seed(rd());
}

void ExcavationMoon::excavate(Player& player, const std::string& equipment) {
    if (!player.hasEquipment(equipment)) {
        std::cout << "You don't have a " << equipment << "!" << std::endl;
        return;
    }

    player.spendFood();

    for (auto& item : player.getInventory()) {
        if (item.type == "equipment" && item.name == equipment && !item.isBroken) {
            std::uniform_int_distribution<int> dist(1, 100);

            if (dist(rng) <= 10) {
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

    std::cout << "Excavating on the Moon with " << equipment << "..." << std::endl;
    std::uniform_int_distribution<int> dist(1, 100);
    int chance = dist(rng);

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

void ExcavationMoon::effect(Player& player) {
    std::cout << "The low gravity maked excavation tricky!" << std::endl;
    std::uniform_int_distribution<int> dist(1, 100);
    int chance = dist(rng);

    if (chance <= 10) {
        const auto& inventory = player.getInventory();

        if (!inventory.empty()) {
            std::uniform_int_distribution<size_t> itemDist(0, inventory.size() - 1);
            size_t index = itemDist(rng);
            std::string itemName = inventory[index].name;
            player.sellItem(index);
            player.spendMoney(-inventory[index].value);
            std::cout << "Low gravity caused you to lose your " << itemName << "!" << std::endl;
        }
    }
}

void ExcavationMoon::printText() const {
    std::cout << "Low gravity, but digging is still tough!\n";
}
