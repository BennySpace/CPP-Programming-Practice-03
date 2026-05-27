#include "race_spa.h"
#include <iostream>

race_spa::race_spa() : race("Belgian Grand Prix at Spa", "Mixed Weather", 150) {
    std::random_device randomDevice;
    mRandomNumberGenerator.seed(randomDevice());
}

void race_spa::drive(player& pPlayer, const std::string& pMod) {
    if (!pPlayer.has_mod(pMod)) {
        std::cout << "You don't have a working " << pMod << " mod!" << std::endl;
        return;
    }

    pPlayer.spend_fuel();

    for (auto& item : pPlayer.get_inventory()) {
        if (item.mType == "equipment" && item.mName == pMod && !item.mIsBroken) {
            std::uniform_int_distribution<int> dist(1, 100);

            if (dist(mRandomNumberGenerator) <= SPA_MOD_WEAR_CHANCE) {
                item.mDurability = std::max(0, item.mDurability - 15);
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

    if (pMod == "Aerodynamics" && chance <= SPA_AW_LOOT_CHANCE) {
        pPlayer.add_item(item("Trophy Cap", "loot", 60));
        std::cout << "You won a Trophy Cap!" << std::endl;
    } else if (pMod == "Engine" && chance <= SPA_HP_LOOT_CHANCE) {
        pPlayer.add_item(item("Suspension Part", "loot", 100));
        std::cout << "You found a Suspension Part!" << std::endl;
    } else if (pMod == "Tires" && chance <= SPA_WG_LOOT_CHANCE) {
        pPlayer.add_item(item("Gearbox Component", "loot", 150));
        std::cout << "You found a Gearbox Component!" << std::endl;
    } else {
        std::cout << "No trophies this time. Try different mods!" << std::endl;
    }
}

void race_spa::effect(player& pPlayer) {
    std::cout << "The changing weather conditions affect your performance" << std::endl;
    std::uniform_int_distribution<int> dist(1, 100);
    int chance = dist(mRandomNumberGenerator);

    if (chance <= SPA_RAIN_FUEL_CHANCE) {
        pPlayer.spend_fuel();
        std::cout << "Rain exhausted you! You consumed 1 extra food." << std::endl;
    }
}

void race_spa::print_text() const {
    std::cout << "You tackle the unpredictable weather at Spa-Francorchamps!\n";
}
