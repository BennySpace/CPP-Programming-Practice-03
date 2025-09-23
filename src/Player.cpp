#include "Player.h"

#include <iostream>
#include <ostream>

Player::Player() : money(500), food(10) {}

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

void Player::addItem(const std::string &item) {
    inventory.push_back(item);
}

bool Player::isGameOver() const {
    return money <= 0 && food <= 0;
}

void Player::showStatus() const {
    std::cout << "Money: " << money << " | Food: " << food << std::endl;
    std::cout << "Inventory:";

    for (const auto& item : inventory) {
        std::cout << item << " , ";
    }

    std::cout << std::endl;
}

