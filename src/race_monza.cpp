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

    const item lootOptions[] = {
        item("Trophy Medal", "loot", 80),
        item("Aero Wing Part", "loot", 200),
        item("Brake System", "loot", 300)
    };
    const char* requiredMods[] = {"Aerodynamics", "Engine", "Tires"};
    const int lootChances[] = {MONZA_AW_LOOT_CHANCE, MONZA_HP_LOOT_CHANCE, MONZA_WG_LOOT_CHANCE};
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
