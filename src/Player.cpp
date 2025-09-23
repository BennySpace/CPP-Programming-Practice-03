#include "Player.h"
#include <iostream>

Player::Player() : money(500), food(10) {
    inventory.push_back(Item("Brush", "equipment", 10));
    inventory.push_back(Item("Shovel", "equipment", 20));
    inventory.push_back(Item("Pickaxe", "equipment", 30));
}

int Player::getMoney() const {
    return money;
}

int Player::getFood() const {
    return food;
}

void Player::addMoney(int amount) {
    money += amount;
}

void Player::spendMoney(int amount) {
    money = std::max(0, money - amount);
}

void Player::addFood(int amount) {
    food += amount;
}

void Player::spendFood() {
    if (food > 0) {
        food--;
    }
}

void Player::addItem(const Item &item) {
    inventory.push_back(item);
}

bool Player::hasEquipment(const std::string &equipment) const {
    for (const auto& item : inventory) {
        if (item.type == "equipment" && item.name == equipment) {
            return true;
        }
    }

    return false;
}

const std::vector<Item> & Player::getInventory() const {
    return inventory;
}

void Player::sellItem(size_t index) {
    if (index < inventory.size() && inventory[index].type == "loot") {
        addMoney(inventory[index].value);
        std::cout << "Sold " << inventory[index].name << " for " << inventory[index].value << " money." << std::endl;
        inventory.erase(inventory.begin() + index);
    } else {
        std::cout << "Invalid item or nor a loot item." << std::endl;
    }
}

bool Player::isGameOver() const {
    return money <= 0 && food <= 0;
}

void Player::showStatus() const {
    std::cout << "Money: " << money << " | Food: " << food << std::endl;
    std::cout << "Inventory:";

    for (const auto& item : inventory) {
        std::cout << item.name << " (" << item.type << "), ";
    }

    std::cout << std::endl;
}

