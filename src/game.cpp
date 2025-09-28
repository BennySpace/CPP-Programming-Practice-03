#include "game.h"
#include "race_monza.h"
#include "race_monaco.h"
#include "race_spa.h"
#include <iostream>
#include <limits>

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

void game::run() {
    while (!mPlayer.is_game_over()) {
        show_main_menu();
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        handle_main_menu(choice);
    }

    std::cout << "Game over! You ran out if money and fuel." << std::endl;
}

void game::show_main_menu() {
    std::cout << "\n=== Paddock Headquarters ===" << std::endl;
    std::cout << "1. Start a race" << std::endl;
    std::cout << "2. Visit the team shop" << std::endl;
    std::cout << "3. Visit the F1 museum" << std::endl;
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
    std::cout << "\n=== Choose Race ===" << std::endl;

    for (size_t i = 0; i < mRaces.size(); ++i) {
        std::cout << i + 1 << ". " << mRaces[i]->get_name()
                  << " (" << mRaces[i]->get_circuit_type()
                  << ", entry fee: " << mRaces[i]->get_cost() << ")" << std::endl;
    }

    std::cout << "Choose a race (0 to return): ";
    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice >= 1 && choice <= static_cast<int>(mRaces.size())) {
        start_race(mRaces[choice - 1]);
        mPlayer.save(mSaveFile);
    }
}

void game::start_race(race* pRace) {
    if (mPlayer.get_money() < pRace->get_cost()) {
        std::cout << "Not enough money for the race!" << std::endl;
        return;
    }

    if (mPlayer.get_food() <= 0) {
        std::cout << "Not enough fuel for the race! Visit the shop to buy fuel." << std::endl;
        return;
    }

    mPlayer.spend_money(pRace->get_cost());
    std::cout << "You embarked on " << pRace->get_name() << "!" << std::endl;
    pRace->print_text();
    pRace->effect(mPlayer);
    mPlayer.save(mSaveFile);

    while (mPlayer.get_food() > 0) {
        std::cout << "\nChoose mod for race:" << std::endl;
        std::cout << "1. Aerodynamics\n2. Engine\n3. Tires\n0. Return to base" << std::endl;
        std::cout << "Choice: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::string equipment;

        switch (choice) {
            case 1: equipment = "Aerodynamics"; break;
            case 2: equipment = "Engine"; break;
            case 3: equipment = "Tires"; break;
            case 4: return;
            default: std::cout << "Invalid choice, try again." << std::endl; continue;
        }

        pRace->drive(mPlayer, equipment);
        mPlayer.show_status();
        mPlayer.save(mSaveFile);
    }

    std::cout << "Out of fuel! Returning to base." << std::endl;
}

void game::visit_shop() {
    std::cout << "\n=== Team Shop ===" << std::endl;
    std::cout << "1. Buy fuel (50 money for 5 units)" << std::endl;
    std::cout << "2. Buy Aerodynamics (10 money)" << std::endl;
    std::cout << "3. Buy Engine (20 money)" << std::endl;
    std::cout << "4. Buy Tires (30 money)" << std::endl;
    std::cout << "5. Sell loot" << std::endl;
    std::cout << "6. Repair mod" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Choose an action: " << std::endl;
    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (choice) {
        case 1: {
            if (mPlayer.get_money() >= 50) {
                mPlayer.spend_money(50);
                mPlayer.add_food(5);
                std::cout << "Purchased 5 units of of fuel" << std::endl;
                mPlayer.save(mSaveFile);
            } else {
                std::cout << "Not enough money." << std::endl;
            }

            break;
        }

        case 2: {
            mPlayer.buy_mod("Aerodynamics", 10);
            if (mPlayer.get_money() >= 10) {
                mPlayer.save(mSaveFile);
            }
            break;
        }

        case 3: {
            mPlayer.buy_mod("Engine", 20);
            if (mPlayer.get_money() >= 20) {
                mPlayer.save(mSaveFile);
            }
            break;
        }

        case 4: {
            mPlayer.buy_mod("Tires", 30);
            if (mPlayer.get_money() >= 30) {
                mPlayer.save(mSaveFile);
            }
            break;
        }

        case 5: {
            const auto& inventory = mPlayer.get_inventory();

            if (inventory.empty()) {
                std::cout << "No items to sell.";
                break;
            }

            std::cout << "Look items: " << std::endl;
            for (size_t i = 0; i < inventory.size(); ++i) {
                if (inventory[i].mType == "loot") {
                    std::cout << i + 1 << ". " << inventory[i].mName
                              << " (value: " << inventory[i].mValue << ")" << std::endl;
                }
            }

            std::cout << "Choose item to sell (0 to cancel): ";
            int itemChoice;
            std::cin >> itemChoice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (itemChoice > 0 && itemChoice < inventory.size()) {
                mPlayer.sell_item(itemChoice - 1);
                mPlayer.save(mSaveFile);
            }

            break;
        }

        case 6: {
            const auto& inventory = mPlayer.get_inventory();
            bool hasBroken = false;

            for (const auto& item : inventory) {
                if (item.mType == "equipment" && item.mIsBroken) {
                    hasBroken = true;
                    break;
                }
            }

            if (!hasBroken) {
                std::cout << "No broken mod to repair." << std::endl;
                break;
            }

            std::cout << "Broken mod: " << std::endl;
            for (size_t i = 0; i < inventory.size(); ++i) {
                if (inventory[i].mType == "equipment" && inventory[i].mIsBroken) {
                    std::cout << i + 1 << ". " << inventory[i].mName << " (repair cost: " << inventory[i].mValue / 2 << ")" << std::endl;
                }
            }

            std::cout << "Choose mod to repair (0 to cancel): ";
            int itemChoice;
            std::cin >> itemChoice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (itemChoice > 0 && itemChoice <= static_cast<int>(inventory.size())) {
                mPlayer.repair_equipment(itemChoice - 1, inventory[itemChoice - 1].mValue / 2);
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
    std::cout << "\n=== F1 Museum ===" << std::endl;
    std::cout << "1. View collection\n2. Donate loot\n3. Return to base" << std::endl;
    std::cout << "Choose an action: ";
    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (choice) {
        case 1: {
            mPlayer.show_museum();
            break;
        }
        case 2: {
            const auto& inventory = mPlayer.get_inventory();
            if (inventory.empty()) {
                std::cout << "No items to donate." << std::endl;
                break;
            }

            std::cout << "Loot items available for donation:" << std::endl;
            for (size_t i = 0; i < inventory.size(); ++i) {
                if (inventory[i].mType == "loot") {
                    std::cout << i + 1 << ". " << inventory[i].mName << " (value: " << inventory[i].mValue
                                       << ")" << std::endl;
                }
            }

            std::cout << "Choose item to donate (0 to cancel): ";
            int itemChoice;
            std::cin >> itemChoice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (itemChoice > 0 && itemChoice <= static_cast<int>(inventory.size())) {
                mPlayer.donate_to_museum(itemChoice - 1);
                mPlayer.save(mSaveFile);
            }

            break;
        }
        case 3: break;
        default: std::cout << "Invalid choice, try again." << std::endl;
    }
}