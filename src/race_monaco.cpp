#include "race_monaco.h"
#include <iostream>

race_monaco::race_monaco() : race("Monaco Grand Prix", "Street Circuit", 100) {
    std::random_device randomDevice;
    mRandomNumberGenerator.seed(randomDevice());
}

void race_monaco::drive(player& pPlayer, const std::string& pMod) {
    if (!pPlayer.has_mod(pMod)) {
        std::cout << "You don't have a working " << pMod << " mod!" << std::endl;
        return;
    }

    pPlayer.spend_fuel();

    for (auto& item : pPlayer.get_inventory()) {
        if (item.mType == "equipment" && item.mName == pMod && !item.mIsBroken) {
            std::uniform_int_distribution<int> dist(1, 100);
            const int chance = dist(mRandomNumberGenerator);

            if (chance <= MONACO_MOD_WEAR_CHANCE) {
                item.mDurability = std::max(0, item.mDurability - 10);
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
    const int chance = dist(mRandomNumberGenerator);

    if (pMod == "Aerodynamics" && chance <= MONACO_AW_LOOT_CHANCE) {
        pPlayer.add_item(item("Podium Hat", "loot", 50));
        std::cout << "You won a Podium Hat!" << std::endl;
    } else if (pMod == "Engine" && chance <= MONACO_HP_LOOT_CHANCE) {
        pPlayer.add_item(item("Engine Part", "loot", 80));
        std::cout << "You found and Engine Part!" << std::endl;
    } else if (pMod == "Tires" && chance <= MONACO_WG_LOOT_CHANCE) {
        pPlayer.add_item(item("Tire Compound", "loot", 120));
        std::cout << "You found a Tire Compound!" << std::endl;
    } else {
        std::cout << "No trophies this time. Try different mods!" << std::endl;
    }
}

void race_monaco::effect(player& pPlayer) {
    std::cout << "The narrow turns demand precision driving." << std::endl;

    std::uniform_int_distribution<int> dist(1, 100);
    const int chance = dist(mRandomNumberGenerator);

    if (chance <= MONACO_CRASH_CHANCE) {
        pPlayer.spend_money(10);
        std::cout << "Water damaged your equipment! You spent 10 money on repairs." << std::endl;
    }
}

void race_monaco::print_text() const {
    std::cout << "You're tired, but you must keep working underwater.\n";
}
