#include "race_monza.h"
#include <iostream>

race_monza::race_monza() : race("Italian Grand Prix at Monza", "High-Speed Circuit", 200) {
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

            if (dist(mRandomNumberGenerator) <= 15) {
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

    if (pMod == "Aerodynamics" && chance <= 15) {
        pPlayer.add_item(item("Trophy Medal", "loot", 80));
        std::cout << "You won a Trophy Medal!" << std::endl;
    } else if (pMod == "Engine" && chance <= 10) {
        pPlayer.add_item(item("Aero Wing Part", "loot", 200));
        std::cout << "You found an Aero Wing Part!" << std::endl;
    } else if (pMod == "Tires" && chance <= 5) {
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

    if (chance <= 10) {
        const auto& inventory = pPlayer.get_inventory();

        if (!inventory.empty()) {
            std::uniform_int_distribution<size_t> itemDist(0, inventory.size() - 1);
            size_t index = itemDist(mRandomNumberGenerator);
            std::string itemName = inventory[index].mName;
            pPlayer.lose_item(index);
            std::cout << "High-speed vibrations caused you to lose your " << itemName << "!" << std::endl;
        }
    }
}

void race_monza::print_text() const {
    std::cout << "You race on the high-speed straights of Monza, the Temple of Speed!\n";
}
