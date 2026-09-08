#include "race_spa.h"
#include <iostream>

race_spa::race_spa() : race("Belgian Grand Prix at Spa", "Mixed Weather", 35) {
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

    const item lootOptions[] = {
        item("Trophy Cap", "loot", 60),
        item("Suspension Part", "loot", 100),
        item("Gearbox Component", "loot", 150)
    };
    const char* requiredMods[] = {"Aerodynamics", "Engine", "Tires"};
    const int lootChances[] = {SPA_AW_LOOT_CHANCE, SPA_HP_LOOT_CHANCE, SPA_WG_LOOT_CHANCE};
    constexpr size_t kLootOptionCount = sizeof(lootOptions) / sizeof(lootOptions[0]);

    std::uniform_int_distribution<size_t> lootDist(0, kLootOptionCount - 1);
    const size_t lootIndex = lootDist(mRandomNumberGenerator);
    const item& targetLoot = lootOptions[lootIndex];

    if (pMod == requiredMods[lootIndex]) {
        std::uniform_int_distribution<int> chanceDist(1, 100);
        if (chanceDist(mRandomNumberGenerator) <= lootChances[lootIndex]) {
            pPlayer.add_item(targetLoot);
            std::cout << "You found a " << targetLoot.mName << "!" << std::endl;
        } else {
            std::cout << "The " << targetLoot.mName << " slipped away this time." << std::endl;
        }
    } else if (pMod == "Aerodynamics") {
        std::cout << "Aerodynamics was too gentle to recover the " << targetLoot.mName << "." << std::endl;
    } else {
        std::cout << pMod << " mod damaged the " << targetLoot.mName << " before you could recover it!" << std::endl;
    }
}

void race_spa::effect(player& pPlayer) {
    std::cout << "The changing weather conditions affect your performance." << std::endl;
    std::uniform_int_distribution<int> dist(1, 100);
    int chance = dist(mRandomNumberGenerator);

    if (chance <= SPA_RAIN_FUEL_CHANCE) {
        pPlayer.spend_fuel();
        std::cout << "Rain exhausted you! You consumed 1 extra fuel." << std::endl;
    }
}

void race_spa::print_text() const {
    std::cout << "You tackle the unpredictable weather at Spa-Francorchamps!\n";
}
