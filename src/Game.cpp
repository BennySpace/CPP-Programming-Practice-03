#include "Game.h"
#include "ExcavationMoon.h"
#include "ExcavationUnderwater.h"
#include "ExcavationVulkano.h"
#include <iostream>
#include <limits>

Game::Game() {
    player.load(saveFile);
    expeditions.push_back(new ExcavationUnderwater());
    expeditions.push_back(new ExcavationVulkano());
    expeditions.push_back(new ExcavationMoon());
}

Game::~Game() {
    for (auto* expedition : expeditions) {
        delete expedition;
    }
}

void Game::run() {
    while (!player.isGameOver()) {
        showMainMenu();
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        handleMainMenu(choice);
    }

    std::cout << "Game over! You ran out if money and food." << std::endl;
}

void Game::showMainMenu() {
    std::cout << "\n=== Paleontologist Base ===" << std::endl;
    std::cout << "1. Go on an expedition" << std::endl;
    std::cout << "2. Visit the shop" << std::endl;
    std::cout << "3. Visit the museum" << std::endl;
    std::cout << "4. Exit game" << std::endl;
    player.showStatus();
    std::cout << "Choose an action: ";
}

void Game::handleMainMenu(int choice) {
    switch (choice) {
        case 1: chooseExpedition(); break;
        case 2: visitShop(); break;
        case 3: visitMuseum(); break;
        case 4: exit(0);
        default: std::cout << "Invalid choice, try again." << std::endl;
    }
}

void Game::chooseExpedition() {
    std::cout << "\n=== Choose Expedition ===" << std::endl;

    for (size_t i = 0; i < expeditions.size(); ++i) {
        std::cout << i + 1 << ". " << expeditions[i]->getName()
                  << " (" << expeditions[i]->getTerrainType()
                  << ", cost: " << expeditions[i]->getCost() << ")" << std::endl;
    }

    std::cout << "Choose an expedition (0 to return): ";
    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice >= 1 && choice <= static_cast<int>(expeditions.size())) {
        startExpedition(expeditions[choice - 1]);
        player.save(saveFile);
    }
}

void Game::startExpedition(Excavation* expedition) {
    if (player.getMoney() >= expedition->getCost()) {
        player.spendMoney(expedition->getCost());
        std::cout << "You embarked on " << expedition->getName() << "!" << std::endl;
        expedition->printText();
        expedition->effect();

        while (player.getFood() > 0) {
            std::cout << "\nChoose equipment for excavation:" << std::endl;
            std::cout << "1. Brush\n2. Shovel\n3. Pickaxe\n4. Return to base" << std::endl;
            std::cout << "Choice: ";
            int choice;
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::string equipment;

            switch (choice) {
                case 1: equipment = "Brush"; break;
                case 2: equipment = "Shovel"; break;
                case 3: equipment = "Pickaxe"; break;
                case 4: return;
                default: std::cout << "Invalid choice, try again." << std::endl; continue;
            }

            expedition->excavate(player, equipment);
            player.showStatus();
            player.save(saveFile);
        }

        std::cout << "Out of food! Returning to base." << std::endl;
    } else {
        std::cout << "Not enough money for the expedition!" << std::endl;
    }
}

void Game::visitShop() {
    std::cout << "Shop: \n1. Buy food (50 money for 5 units)\n2. Sell loot" << std::endl;
    std::cout << "Choose an action (0 to return): ";
    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 1 && player.getMoney() >= 50) {
        player.spendMoney(50);
        player.addFood(5);
        std::cout << "Purchased 5 units of food." << std::endl;
        player.save(saveFile);
    } else if (choice == 2) {
        const auto& inventory = player.getInventory();

        if (inventory.empty()) {
            std::cout << "No items to sell." << std::endl;
            return;
        }

        std::cout << "Loot items:" << std::endl;

        for (size_t i = 0; i < inventory.size(); ++i) {
            if (inventory[i].type == "loot") {
                std::cout << i + 1 << ". " << inventory[i].name << " (value: " << inventory[i].value << ")" << ")" << std::endl;
            }
        }

        std::cout << "Choose item to sell (0 to cancel): ";
        int itemChoice;
        std::cin >> itemChoice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (itemChoice > 0 && itemChoice <= static_cast<int>(inventory.size())) {
            player.sellItem(itemChoice - 1);
            player.save(saveFile);
        }
    } else if (choice != 0) {
        std::cout << "Not enough money or invalid choice." << std::endl;
    }
}

void Game::visitMuseum() {
    std::cout << "\n=== Museum ===" << std::endl;
    std::cout << "1. View collection\n2. Donate loot\n3. Return to base" << std::endl;
    std::cout << "Choose an action: ";
    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (choice) {
        case 1: {
            player.showMuseum();
            break;
        }
        case 2: {
            const auto& inventory = player.getInventory();
            if (inventory.empty()) {
                std::cout << "No items to donate." << std::endl;
                break;
            }

            std::cout << "Loot items available for doantion:" << std::endl;
            for (size_t i = 0; i < inventory.size(); ++i) {
                if (inventory[i].type == "loot") {
                    std::cout << i + 1 << ". " << inventory[i].name << " (value: " << inventory[i].value
                                       << ")" << std::endl;
                }
            }

            std::cout << "Choose item to donate (0 to cancel): ";
            int itemChoice;
            std::cin >> itemChoice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (itemChoice > 0 && itemChoice <= static_cast<int>(inventory.size())) {
                player.donateToMuseum(itemChoice - 1);
                player.save(saveFile);
            }

            break;
        }
        case 3: break;
        default: std::cout << "Invalid choice, try again." << std::endl;
    }
}
