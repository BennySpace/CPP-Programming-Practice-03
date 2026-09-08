#include "race_monaco.h"
#include <iostream>

race_monaco::race_monaco() : race("Monaco Grand Prix", "Street Circuit", 25) {
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

    const item lootOptions[] = {
        item("Podium Hat", "loot", 50),
        item("Engine Part", "loot", 80),
        item("Tire Compound", "loot", 120)
    };
    const char* requiredMods[] = {"Aerodynamics", "Engine", "Tires"};
    const int lootChances[] = {MONACO_AW_LOOT_CHANCE, MONACO_HP_LOOT_CHANCE, MONACO_WG_LOOT_CHANCE};
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

void race_monaco::effect(player& pPlayer) {
    std::cout << "The narrow turns demand precision driving." << std::endl;

    std::uniform_int_distribution<int> dist(1, 100);
    const int chance = dist(mRandomNumberGenerator);

    if (chance <= MONACO_CRASH_CHANCE) {
        pPlayer.spend_money(10);
        std::cout << "A crash damaged your equipment! You spent 10 money on repairs." << std::endl;
    }
}

void race_monaco::print_text() const {
    std::cout << "You face Monaco's tight barriers and unforgiving corners.\n";
}
