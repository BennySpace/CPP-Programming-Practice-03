#include "Player.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>

using json = nlohmann::json;

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

bool Player::save(const std::string &filename) const {
    json j;
    j["money"] = money;
    j["food"] = food;
    j["inventory"] = json::array();

    for (const auto& item : inventory) {
        j["inventory"].push_back({
            {"name", item.name},
            {"type", item.type},
            {"value", item.value}
        });
    }

    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cout << "ERROR: Could not open file " << filename << " for saving." << std::endl;

        return false;
    }

    file << j.dump(4);
    file.close();
    std::cout << "Game saved successfully.";

    return true;
}

bool Player::load(const std::string &filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "No save file found, starting new game." << std::endl;

        return false;
    }

    try {
        json j;
        file >> j;
        file.close();

        money = j.value("money", 500);
        food = j.value("food", 10);
        inventory.clear();

        for (const auto& item : j["inventory"]) {
            inventory.push_back(Item(
                item.value("name", ""),
                item.value("type", ""),
                item.value("value", 0)
            ));
        }
    } catch (const json::exception& e) {
        std::cout << "Error loading save file: " << e.what() << std::endl;
        file.close();
        return false;
    }

    std::cout << "Game loaded successfully." << std::endl;

    return true;
}