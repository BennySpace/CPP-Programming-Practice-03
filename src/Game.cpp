#include "Game.h"
#include "ExcavationMoon.h"
#include "ExcavationUnderwater.h"
#include "ExcavationVulkano.h"
#include <iostream>

Game::Game() {
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
        handleMainMenu(choice);
    }

    std::cout << "Game over! You ran out if money and food.\n";
}

void Game::showMainMenu() {
    std::cout << "\n=== Paleontologist Base ===\n";
    std::cout << "1. Go on an expedition\n";
    std::cout << "2. Visit the shop\n";
    std::cout << "3. Visit the museum\n";
    std::cout << "4. Exit game\n";
    player.showStatus();
    std::cout << "Choose an action: ";
}

void Game::handleMainMenu(int choice) {
    switch (choice) {
        case 1: chooseExpedition(); break;
        case 2: visitShop(); break;
        case 3: visitMuseum(); break;
        case 4: exit(0);
        default: std::cout << "Invalid choice, try again.\n";
    }
}

void Game::chooseExpedition() {
    std::cout << "\n=== Choose Expedition ===\n";

    for (size_t i = 0; i < expeditions.size(); ++i) {
        std::cout << i + 1 << ". " << expeditions[i]->getName()
                  << " (" << expeditions[i]->getTerrainType()
                  << ", cost: " << expeditions[i]->getCost() << ")\n";
    }

    std::cout << "Choose an expedition (0 to return): ";
    int choice;
    std::cin >> choice;

    if (choice >= 1 && choice <= static_cast<int>(expeditions.size())) {
        startExpedition(expeditions[choice - 1]);
    }
}

void Game::startExpedition(Excavation *expedition) {
    if (player.getMoney() >= expedition->getCost()) {
        player.spendMoney(expedition->getCost());
        std::cout << " You embarked on " << expedition->getName() << "!\n";
        expedition->printText();
        expedition->excavate();
        expedition->effect();
        player.spendFood();
    } else {
        std::cout << "Not enough money for the expedition!\n";
    }
}

void Game::visitShop() {
    std::cout << "Shop: \n1. Buy food (50 money for 5 units)\n";
    std::cout << "Choose an action (0 to return): ";
    int choice;
    std::cin >> choice;

    if (choice == 1 && player.getMoney() >= 50) {
        player.spendMoney(50);
        player.addFood(5);
        std::cout << "Purchased 5 units of food.\n";
    } else if (choice != 0) {
        std::cout << "Not enough money or invalid choice.\n";
    }
}

void Game::visitMuseum() {
    std::cout << "Museum: it's empty for now, but soon you'll see your loot here!\n";
}
