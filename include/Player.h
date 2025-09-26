#ifndef PLAYER_H
#define PLAYER_H

#include "Item.h"
#include <string>
#include <vector>

class Player {
private:
    int money;
    int food;
    std::vector<Item> inventory;
    std::vector<Item> museumCollection;

public:
    Player();
    int getMoney() const;
    int getFood() const;
    void addMoney(int amount);
    void spendMoney(int amount);
    void addFood(int amount);
    void spendFood();
    void addItem(const Item& item);
    bool hasEquipment(const std::string& equipment) const;
    const std::vector<Item>& getInventory() const;
    void sellItem(size_t index);
    bool isGameOver() const;
    void showStatus() const;
    bool save(const std::string& filename) const;
    bool load(const std::string& filename);
    void donateToMuseum(size_t index);
    void showMuseum() const;
    bool buyEquipment(const std::string& equipmentName, int cost);
    bool repairEquipment(size_t index, int cost);
};

#endif // PLAYER_H