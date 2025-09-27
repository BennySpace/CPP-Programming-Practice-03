#include "Player.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>

using json = nlohmann::json;

Player::Player() : money(500), food(10) {
    inventory.push_back(Item("Brush", "equipment", 10, "", 100, false));
    inventory.push_back(Item("Shovel", "equipment", 20, "", 100, false));
    inventory.push_back(Item("Pickaxe", "equipment", 30, "", 100, false));
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
        std::cout << item.name << " (" << item.type;
        if (item.type == "equipment") {
            std::cout << ", durability: " << item.durability;

            if (item.isBroken) {
                std::cout << ", BROKEN";
            }

            std::cout << "), ";
        }

        std::cout << std::endl;
    }
}

bool Player::save(const std::string &filename) const {
    json j;
    j["money"] = money;
    j["food"] = food;
    j["inventory"] = json::array();

    for (const auto& item : inventory) {
        json itemJson = {
            {"name", item.name},
            {"type", item.type},
            {"value", item.value}
        };

        if (item.type == "equipment") {
            itemJson["durability"] = item.durability;
            itemJson["isBroken"] = item.isBroken;
        }

        j["inventory"].push_back(itemJson);
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
            Item newItem(
                item.value("name", ""),
                item.value("type", ""),
                item.value("value", 0)
            );

            if (item.value("type", "") == "equipment") {
                newItem.durability = item.value("durability", 100);
                newItem.isBroken = item.value("isBroken", false);
            }

            inventory.push_back(newItem);
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

    size_t exhibitCount = museumCollection.size();
    if (exhibitCount >= 9 && std::find(museumRewards.begin(), museumRewards.end(), 9) == museumRewards.end()) {
        addMoney(300);
        museumRewards.push_back(9);
        std::cout << "Congratulations! You collected 9 exhibits and earned a 300 money reward!" << std::endl;
    } else if (exhibitCount >= 6 && std::find(museumRewards.begin(), museumRewards.end(), 6) == museumRewards.end()) {
        addMoney(200);
        museumRewards.push_back(6);
        std::cout << "Congratulations! You collected 6 exhibits and earned a 200 money reward!" << std::endl;
    } else if (exhibitCount >= 3 && std::find(museumRewards.begin(), museumRewards.end(), 3) == museumRewards.end()) {
        addMoney(100);
        museumRewards.push_back(3);
        std::cout << "Congratulations! You collected 3 exhibits and earned a 100 money reward!" << std::endl;
    }

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

bool Player::repairEquipment(size_t index, int cost) {
    if (index >= inventory.size() || inventory[index].type != "equipment") {
        std::cout << "Invalid item or not equipment." << std::endl;
        return false;
    }

    if (!inventory[index].isBroken) {
        std::cout << inventory[index].name << " is broken." << std::endl;
        return false;
    }

    if (getMoney() < cost) {
        std::cout << "Not enough money to repair " << inventory[index].name << "!" << std::endl;
        return false;
    }

    spendMoney(cost);
    inventory[index].isBroken = false;
    inventory[index].durability = 100;
    std::cout << "Repaired " << inventory[index].name << " for " << cost << " money." << std::endl;

    return true;
}

void Player::reset() {
    money = 500;
    food = 10;
    inventory.clear();
    museumCollection.clear();
    museumRewards.clear();
    inventory.push_back(Item("Brush", "equipment", 10, "", 100, false));
    inventory.push_back(Item("Shovel", "equipment", 20, "", 100, false));
    inventory.push_back(Item("Pickaxe", "equipment", 30, "", 100, false));
    std::cout << "Started a new game!" << std::endl;
}
