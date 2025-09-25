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

    j["museum"] = json::array();
    for (const auto& exhibit : museumCollection) {
        j["museum"].push_back({
            {"name", exhibit.name},
            {"type", exhibit.type},
            {"value", exhibit.value},
            {"description", exhibit.description}
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

        museumCollection.clear();
        for (const auto& exhibit : j.value("museum", json::array())) {
            museumCollection.push_back(Item(
                exhibit.value("name", ""),
                exhibit.value("type", ""),
                exhibit.value("value", 0),
                exhibit.value("description", "")
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

void Player::donateToMuseum(size_t index) {
    if (index >= inventory.size() || inventory[index].type != "loot") {
        std::cout << "Invalid item or not a loot item." << std::endl;
        return;
    }

    for (const auto& exhibit : museumCollection) {
        if (exhibit.name == inventory[index].name) {
            std::cout << "This item is already in the museum!" << std::endl;
            return;
        }
    }

    Item exhibit = inventory[index];
    if (exhibit.name == "Ammonite") {
        exhibit.description = "A spiral shell from an ancient marine creature.";
    } else if (exhibit.name == "Marine Fossil") {
        exhibit.description = "A fossilized remnant of sea creature.";
    } else if (exhibit.name == "Dinosaur Bone") {
        exhibit.description = "A well-preserved dinosaur bone from a volcanic expedition.";
    } else if (exhibit.name == "Fossil Fragment") {
        exhibit.description = "A small piece of a larger fossil.";
    } else if (exhibit.name == "Cosmic Fossil") {
        exhibit.description = "A rare fossil";
    } else if (exhibit.name == "Lunar Rock") {
        exhibit.description = "A rock sample from the moon.";
    }

    museumCollection.push_back(exhibit);
    std::cout << "Donated " << exhibit.name << " to the museum!" << std::endl;
    inventory.erase(inventory.begin() + index);
}

void Player::showMuseum() const {
    if (museumCollection.empty()) {
        std::cout << "The museum is empty. Donate some loot to display!";
        return;
    }

    std::cout << "\n=== Museum Collection ===" << std::endl;
    for (const auto& exhibit : museumCollection) {
        std::cout << "- " << exhibit.name << " (" << exhibit.type << ", value: " << exhibit.value << ")" << std::endl;
        std::cout << " Description: " << exhibit.description << std::endl;
    }
}

bool Player::buyEquipment(const std::string &equipmentName, int cost) {
    if (hasEquipment(equipmentName)) {
        std::cout << "You already have a " << equipmentName << " !" << std::endl;
        return false;
    }

    if (getMoney() < cost) {
        std::cout << "Not enough money to buy " << equipmentName << "!" << std::endl;
        return false;
    }

    spendMoney(cost);
    addItem(Item(equipmentName, "equipment", cost));
    std::cout << "Purchased " << equipmentName << " for " << cost << " money." << std::endl;
    return true;
}