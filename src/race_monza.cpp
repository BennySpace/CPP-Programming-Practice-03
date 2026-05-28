#include "race_monza.h"
#include <iostream>

race_monza::race_monza() : race("Italian Grand Prix at Monza", "High-Speed Circuit", 45) {
    std::random_device randomDevice;
    mRandomNumberGenerator.seed(randomDevice());
}

void race_monza::drive(player& pPlayer, const std::string& pMod) {
    if (!pPlayer.has_mod(pMod)) {
        std::cout << "You don't have a working " << pMod << " mod!" << std::endl;
        return;
    }

    pPlayer.spend_fuel();

    for (auto& item : pPlayer.get_inventory()) {
        if (item.mType == "equipment" && item.mName == pMod && !item.mIsBroken) {
            std::uniform_int_distribution<int> dist(1, 100);

            if (dist(mRandomNumberGenerator) <= MONZA_MOD_WEAR_CHANCE) {
                item.mDurability = std::max(0, item.mDurability - 20);
                std::cout << pMod << " mod took wear! Durability now: " << item.mDurability << std::endl;

                if (item.mDurability <= 0) {
                    item.mIsBroken = true;
                    std::cout << pMod << " mod is broken and needs repair!" << std::endl;
                }
            }

            break;
        }
    }

    std::uniform_int_distribution<int> dist(1, 100);
    int chance = dist(mRandomNumberGenerator);

    if (pMod == "Aerodynamics" && chance <= MONZA_AW_LOOT_CHANCE) {
        pPlayer.add_item(item("Trophy Medal", "loot", 80));
        std::cout << "You won a Trophy Medal!" << std::endl;
    } else if (pMod == "Engine" && chance <= MONZA_HP_LOOT_CHANCE) {
        pPlayer.add_item(item("Aero Wing Part", "loot", 200));
        std::cout << "You found an Aero Wing Part!" << std::endl;
    } else if (pMod == "Tires" && chance <= MONZA_WG_LOOT_CHANCE) {
        pPlayer.add_item(item("Brake System", "loot", 300));
        std::cout << "You found a Brake System!" << std::endl;
    } else {
        std::cout << "No trophies this time. Try different mods!" << std::endl;
    }
}

void race_monza::effect(player& pPlayer) {
    std::cout << "The high speeds test your car's limits!" << std::endl;
    std::uniform_int_distribution<int> dist(1, 100);
    int chance = dist(mRandomNumberGenerator);

    if (chance <= MONZA_ITEM_LOSS_CHANCE) {
        const auto& inventory = pPlayer.get_inventory();
        std::vector<size_t> lootIndexes;

        for (size_t i = 0; i < inventory.size(); ++i) {
            if (inventory[i].mType == "loot") {
                lootIndexes.push_back(i);
            }
        }

        if (!lootIndexes.empty()) {
            std::uniform_int_distribution<size_t> itemDist(0, lootIndexes.size() - 1);
            size_t index = lootIndexes[itemDist(mRandomNumberGenerator)];
            std::string itemName = inventory[index].mName;
            pPlayer.lose_item(index);
            std::cout << "High-speed vibrations caused you to lose your " << itemName << "!" << std::endl;
        }
    }
}

void race_monza::print_text() const {
    std::cout << "You race on the high-speed straights of Monza, the Temple of Speed!\n";
}
