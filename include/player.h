#ifndef PLAYER_H
#define PLAYER_H

#include "item.h"
#include <string>
#include <vector>

class player {
private:
    int mMoney;
    int mFood;
    std::vector<item> mInventory;
    std::vector<item> mMuseumCollection;
    std::vector<int> mMuseumRewards;

public:
    player();
    int get_money() const;
    int get_food() const;
    void add_money(int pAmount);
    void spend_money(int pAmount);
    void add_food(int pAmount);
    void spend_food();
    void add_item(const item& pItem);
    bool has_mod(const std::string& pMod) const;
    const std::vector<item>& get_inventory() const;
    void sell_item(size_t pIndex);
    bool is_game_over() const;
    void show_status() const;
    bool save(const std::string& pFilename) const;
    bool load(const std::string& pFilename);
    void donate_to_museum(size_t pIndex);
    void show_museum() const;
    bool buy_mod(const std::string& pModName, int pCost);
    bool repair_equipment(size_t pIndex, int pCost);
};

#endif // PLAYER_H