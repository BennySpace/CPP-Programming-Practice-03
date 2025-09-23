#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>

class Player {
private:
    int money;
    int food;
    std::vector<std::string> inventory;

public:
    Player();
    int getMoney() const;
    int getFood() const;
    void addMoney(int amount);
    void spendMoney(int amount);
    void addFood(int amount);
    void spendFood();
    void addItem(const std::string &item);
    bool isGameOver() const;
    void showStatus() const;
};


#endif // PLAYER_H