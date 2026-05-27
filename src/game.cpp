#include "game.h"
#include "race_monza.h"
#include "race_monaco.h"
#include "race_spa.h"
#include <functional>
#include <iostream>
#include <limits>

namespace {
constexpr int kFuelPurchaseCost = 50;
constexpr int kFallbackModPurchaseCost = 10;

int read_int() {
    int value;

    while (!(std::cin >> value)) {
        std::cout << "Invalid input. Please enter a number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return value;
}

std::vector<size_t> collect_inventory_indexes(const std::vector<item>& pInventory,
                                              const std::function<bool(const item&)>& pPredicate) {
    std::vector<size_t> indexes;

    for (size_t i = 0; i < pInventory.size(); ++i) {
        if (pPredicate(pInventory[i])) {
            indexes.push_back(i);
        }
    }

    return indexes;
}

void print_inventory_selection(const std::vector<item>& pInventory,
                               const std::vector<size_t>& pIndexes,
                               const std::string& pHeader,
                               const std::function<std::string(const item&)>& pDetails) {
    std::cout << pHeader << std::endl;
    for (size_t i = 0; i < pIndexes.size(); ++i) {
        const auto& listedItem = pInventory[pIndexes[i]];
        std::cout << i + 1 << ". " << listedItem.mName << pDetails(listedItem) << std::endl;
    }
}

std::vector<size_t> collect_inventory_indexes_by_type(const std::vector<item>& pInventory, const std::string& pType) {
    return collect_inventory_indexes(pInventory, [&pType](const item& pItem) {
        return pItem.mType == pType;
    });
}

std::vector<size_t> collect_broken_equipment_indexes(const std::vector<item>& pInventory) {
    return collect_inventory_indexes(pInventory, [](const item& pItem) {
        return pItem.mType == "equipment" && pItem.mIsBroken;
    });
}

int choose_listed_item(const std::vector<size_t>& pIndexes, const std::string& pPrompt) {
    if (pIndexes.empty()) {
        return -1;
    }

    std::cout << pPrompt;
    const int itemChoice = read_int();

    if (itemChoice > 0 && itemChoice <= static_cast<int>(pIndexes.size())) {
        return itemChoice - 1;
    }

    if (itemChoice != 0) {
        std::cout << "Invalid choice, try again." << std::endl;
    }

    return -1;
}

std::string choose_race_mod() {
    while (true) {
        std::cout << "\nChoose car modification for race:" << std::endl;
        std::cout << "1. Aerodynamics (advanced wing profiles for better cornering grip)" << std::endl;
        std::cout << "2. Engine (tuned for maximum power output)" << std::endl;
        std::cout << "3. Tires (specialized compound for changing track conditions)" << std::endl;
        std::cout << "0. Return to HQ" << std::endl;
        std::cout << "Choice: ";

        switch (read_int()) {
            case 1: return "Aerodynamics";
            case 2: return "Engine";
            case 3: return "Tires";
            case 0: return "";
            default:
                std::cout << "Invalid choice, try again." << std::endl;
        }
    }
}
}

game::game() {
    mPlayer.load(mSaveFile);
    mRaces.push_back(new race_monaco());
    mRaces.push_back(new race_spa());
    mRaces.push_back(new race_monza());
}

game::~game() {
    for (auto* races : mRaces) {
        delete races;
    }
}

bool game::can_player_continue() const {
    int availableMoney = mPlayer.get_money();
    bool hasWorkingEquipment = false;
    int cheapestEquipmentRecovery = kFallbackModPurchaseCost;

    for (const auto& item : mPlayer.get_inventory()) {
        if (item.mType == "loot") {
            availableMoney += item.mValue;
            continue;
        }

        if (item.mType == "equipment") {
            if (!item.mIsBroken) {
                hasWorkingEquipment = true;
            } else {
                cheapestEquipmentRecovery = std::min(cheapestEquipmentRecovery, item.mValue / 2);
            }
        }
    }

    int cheapestRaceFee = std::numeric_limits<int>::max();
    for (const auto* pRace : mRaces) {
        cheapestRaceFee = std::min(cheapestRaceFee, pRace->get_fee());
    }

    const int fuelCost = mPlayer.get_fuel() > 0 ? 0 : kFuelPurchaseCost;
    const int equipmentCost = hasWorkingEquipment ? 0 : cheapestEquipmentRecovery;

    return availableMoney >= fuelCost + equipmentCost + cheapestRaceFee;
}

void game::run() {
    while (can_player_continue()) {
        show_main_menu();
        handle_main_menu(read_int());
    }

    std::cout << "Game over! You don't have enough resources to keep racing." << std::endl;
}

void game::show_main_menu() {
    system("CLS");
    std::cout << R"(
 ______                    __ __  ____
/_  __/__ ___ ___ _       / // / / __ \
 / / / -_) _ `/  ' \     / _  / / /_/ /
/_/  \__/\_,_/_/_/_/    /_//_/  \___\_\
    )" << std::endl;
    std::cout << "1. Enter a Grand-Prix" << std::endl;
    std::cout << "2. Visit the Team Shop" << std::endl;
    std::cout << "3. Visit the F1 Museum" << std::endl;
    std::cout << "0. Exit game" << std::endl;
    mPlayer.show_status();
    std::cout << "Choose an action: ";
}

void game::handle_main_menu(int pChoice) {
    switch (pChoice) {
        case 1: choose_race(); break;
        case 2: visit_shop(); break;
        case 3: visit_museum(); break;
        case 0: mPlayer.save(mSaveFile); exit(0);
        default: std::cout << "Invalid choice, try again." << std::endl;
    }
}

void game::choose_race() {
    system("CLS");
    std::cout << R"(
  _____                 __        ___      _
 / ___/______ ____  ___/ / ____  / _ \____(_)_ __
/ (_ / __/ _ `/ _ \/ _  / /___/ / ___/ __/ /\ \ /
\___/_/  \_,_/_//_/\_,_/       /_/  /_/ /_//_\_\
    )" << std::endl;

    for (size_t i = 0; i < mRaces.size(); ++i) {
        std::cout << i + 1 << ". " << mRaces[i]->get_name()
                  << " (" << mRaces[i]->get_circuit_type()
                  << ", entry fee: " << mRaces[i]->get_fee() << ")" << std::endl;
    }

    std::cout << "Choose a race (0 to return): ";
    int choice = read_int();

    if (choice >= 1 && choice <= static_cast<int>(mRaces.size())) {
        start_race(mRaces[choice - 1]);
        mPlayer.save(mSaveFile);
    }
}

void game::start_race(race* pRace) {
    const std::string equipment = choose_race_mod();

    if (equipment.empty()) {
        return;
    }

    if (!mPlayer.has_mod(equipment)) {
        std::cout << "You don't have a working " << equipment << " mod!" << std::endl;
        return;
    }

    if (mPlayer.get_money() < pRace->get_fee()) {
        std::cout << "Not enough money for the race!" << std::endl;
        return;
    }

    if (mPlayer.get_fuel() <= 0) {
        std::cout << "Not enough fuel for the race! Visit the shop to buy fuel." << std::endl;
        return;
    }

    mPlayer.spend_money(pRace->get_fee());
    std::cout << "You entered the " << pRace->get_name() << "!" << std::endl;
    pRace->print_text();
    pRace->effect(mPlayer);
    if (mPlayer.get_fuel() <= 0) {
        std::cout << "Race conditions drained your fuel before the main lap. Returning to HQ." << std::endl;
        mPlayer.save(mSaveFile);
        return;
    }

    pRace->drive(mPlayer, equipment);
    mPlayer.show_status();
    mPlayer.save(mSaveFile);
}

void game::visit_shop() {
    system("CLS");
    std::cout << R"(
 ______                   ______
/_  __/__ ___ ___ _      / __/ /  ___  ___
 / / / -_) _ `/  ' \    _\ \/ _ \/ _ \/ _ \
/_/  \__/\_,_/_/_/_/   /___/_//_/\___/ .__/
                                    /_/
    )" << std::endl;
    std::cout << "1. Buy fuel (50 money for 5 units)" << std::endl;
    std::cout << "2. Buy Aerodynamics (10 money)" << std::endl;
    std::cout << "3. Buy Engine (20 money)" << std::endl;
    std::cout << "4. Buy Tires (30 money)" << std::endl;
    std::cout << "5. Sell loot" << std::endl;
    std::cout << "6. Repair mod" << std::endl;
    std::cout << "0. Return to HQ" << std::endl;
    std::cout << "Choose an action: " << std::endl;
    int choice = read_int();

    switch (choice) {
        case 1: {
            if (mPlayer.get_money() >= 50) {
                mPlayer.spend_money(50);
                mPlayer.add_fuel(5);
                std::cout << "Purchased 5 units of fuel." << std::endl;
                mPlayer.save(mSaveFile);
            } else {
                std::cout << "Not enough money." << std::endl;
            }

            break;
        }

        case 2: {
            if (mPlayer.buy_mod("Aerodynamics", 10)) {
                mPlayer.save(mSaveFile);
            }
            break;
        }

        case 3: {
            if (mPlayer.buy_mod("Engine", 20)) {
                mPlayer.save(mSaveFile);
            }
            break;
        }

        case 4: {
            if (mPlayer.buy_mod("Tires", 30)) {
                mPlayer.save(mSaveFile);
            }
            break;
        }

        case 5: {
            const auto& inventory = mPlayer.get_inventory();
            const auto lootIndexes = collect_inventory_indexes_by_type(inventory, "loot");

            if (lootIndexes.empty()) {
                std::cout << "No items to sell.";
                break;
            }

            print_inventory_selection(inventory, lootIndexes, "Loot items:", [](const item& pItem) {
                return " (value: " + std::to_string(pItem.mValue) + ")";
            });
            const int itemChoice = choose_listed_item(lootIndexes, "Choose item to sell (0 to cancel): ");

            if (itemChoice >= 0) {
                mPlayer.sell_item(lootIndexes[itemChoice]);
                mPlayer.save(mSaveFile);
            }

            break;
        }

        case 6: {
            const auto& inventory = mPlayer.get_inventory();
            const auto brokenEquipmentIndexes = collect_broken_equipment_indexes(inventory);

            if (brokenEquipmentIndexes.empty()) {
                std::cout << "No broken mod to repair." << std::endl;
                break;
            }

            print_inventory_selection(inventory, brokenEquipmentIndexes, "Broken mods:", [](const item& pItem) {
                return " (repair cost: " + std::to_string(pItem.mValue / 2) + ")";
            });
            const int itemChoice = choose_listed_item(brokenEquipmentIndexes, "Choose mod to repair (0 to cancel): ");

            if (itemChoice >= 0) {
                const size_t inventoryIndex = brokenEquipmentIndexes[itemChoice];
                mPlayer.repair_equipment(inventoryIndex, inventory[inventoryIndex].mValue / 2);
                mPlayer.save(mSaveFile);
            }

            break;
        }

        case 0: {
            break;
        }

        default: {
            std::cout << "Invalid choice." << std::endl;
        }
    }
}

void game::visit_museum() {
    system("CLS");
    std::cout << R"(
   ____  ___    __  ___
  / __/ <  /   /  |/  /_ _____ ___ __ ____ _
 / _/   / /   / /|_/ / // (_-</ -_) // /  ' \
/_/    /_/   /_/  /_/\_,_/___/\__/\_,_/_/_/_/
    )" << std::endl;
    std::cout << "1. View collection" << std::endl;
    std::cout << "2. Donate loot" << std::endl;
    std::cout << "0. Return to HQ" << std::endl;
    std::cout << "Choose an action: ";
    int choice = read_int();

    switch (choice) {
        case 1: {
            mPlayer.show_museum();
            break;
        }
        case 2: {
            const auto& inventory = mPlayer.get_inventory();
            const auto lootIndexes = collect_inventory_indexes_by_type(inventory, "loot");

            if (lootIndexes.empty()) {
                std::cout << "No items to donate." << std::endl;
                break;
            }

            print_inventory_selection(inventory, lootIndexes, "Loot items available for donation:", [](const item& pItem) {
                return " (value: " + std::to_string(pItem.mValue) + ")";
            });
            const int itemChoice = choose_listed_item(lootIndexes, "Choose item to donate (0 to cancel): ");

            if (itemChoice >= 0) {
                mPlayer.donate_to_museum(lootIndexes[itemChoice]);
                mPlayer.save(mSaveFile);
            }

            break;
        }
        case 0: break;
        default: std::cout << "Invalid choice, try again." << std::endl;
    }
}
