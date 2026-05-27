#include "player.h"
#include "nlohmann/json.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

using json = nlohmann::json;

namespace {
constexpr int kDefaultEquipmentDurability = 100;

item create_default_equipment(const std::string& pName, int pCost) {
    return item(pName, "equipment", pCost, "", kDefaultEquipmentDurability, false);
}
}

player::player() : mMoney(500), mFuel(10) {
    mInventory.push_back(create_default_equipment("Aerodynamics", 10));
    mInventory.push_back(create_default_equipment("Engine", 20));
    mInventory.push_back(create_default_equipment("Tires", 30));
}

int player::get_money() const {
    return mMoney;
}

int player::get_fuel() const {
    return mFuel;
}

void player::add_money(int pAmount) {
    mMoney += pAmount;
}

void player::spend_money(int pAmount) {
    mMoney = std::max(0, mMoney - pAmount);
}

void player::add_fuel(int pAmount) {
    mFuel += pAmount;
}

void player::spend_fuel() {
    if (mFuel > 0) {
        mFuel--;
    }
}

void player::add_item(const item &pItem) {
    mInventory.push_back(pItem);
}

bool player::has_mod(const std::string &pMod) const {
    for (const auto& item : mInventory) {
        if (item.mType == "equipment" && item.mName == pMod && !item.mIsBroken) {
            return true;
        }
    }

    return false;
}

const std::vector<item> & player::get_inventory() const {
    return mInventory;
}

int player::get_best_loot_value() const {
    int totalSaleValue = 0;
    std::unordered_set<std::string> museumItems;
    std::unordered_map<std::string, int> cheapestDonationByName;

    for (const auto& exhibit : mMuseumCollection) {
        museumItems.insert(exhibit.mName);
    }

    for (const auto& item : mInventory) {
        if (item.mType != "loot") {
            continue;
        }

        totalSaleValue += item.mValue;

        if (museumItems.find(item.mName) != museumItems.end()) {
            continue;
        }

        const auto existingDonation = cheapestDonationByName.find(item.mName);
        if (existingDonation == cheapestDonationByName.end() || item.mValue < existingDonation->second) {
            cheapestDonationByName[item.mName] = item.mValue;
        }
    }

    std::vector<int> donationCosts;
    donationCosts.reserve(cheapestDonationByName.size());
    for (const auto& [_, value] : cheapestDonationByName) {
        donationCosts.push_back(value);
    }

    std::sort(donationCosts.begin(), donationCosts.end());

    int bestLootValue = totalSaleValue;
    int donatedCount = 0;
    int donatedSaleValue = 0;
    const size_t existingExhibits = mMuseumCollection.size();

    for (const int donationCost : donationCosts) {
        donatedSaleValue += donationCost;
        donatedCount++;

        int museumRewardValue = 0;
        for (const int milestone : {3, 6, 9}) {
            if (existingExhibits < static_cast<size_t>(milestone) &&
                existingExhibits + static_cast<size_t>(donatedCount) >= static_cast<size_t>(milestone) &&
                std::find(mMuseumRewards.begin(), mMuseumRewards.end(), milestone) == mMuseumRewards.end()) {
                museumRewardValue += milestone == 3 ? 100 : (milestone == 6 ? 200 : 300);
            }
        }

        bestLootValue = std::max(bestLootValue, totalSaleValue - donatedSaleValue + museumRewardValue);
    }

    return bestLootValue;
}

bool player::lose_item(size_t pIndex) {
    if (pIndex < mInventory.size()) {
        mInventory.erase(mInventory.begin() + pIndex);
        return true;
    }

    std::cout << "Invalid item." << std::endl;
    return false;
}

void player::sell_item(size_t pIndex) {
    if (pIndex < mInventory.size() && mInventory[pIndex].mType == "loot") {
        add_money(mInventory[pIndex].mValue);
        std::cout << "Sold " << mInventory[pIndex].mName << " for " << mInventory[pIndex].mValue << " money." << std::endl;
        mInventory.erase(mInventory.begin() + pIndex);
    } else {
        std::cout << "Invalid item or not a loot item." << std::endl;
    }
}

bool player::is_game_over() const {
    return mMoney <= 0 && mFuel <= 0;
}

void player::show_status() const {
    std::cout << "Money: " << mMoney << " | Fuel: " << mFuel << std::endl;
    std::cout << "Inventory:" << std::endl;

    if (mInventory.empty()) {
        std::cout << "- Empty" << std::endl;
        return;
    }

    for (const auto& item : mInventory) {
        std::cout << "- " << item.mName << " (" << item.mType;

        if (item.mType == "equipment") {
            std::cout << ", durability: " << item.mDurability;

            if (item.mIsBroken) {
                std::cout << ", BROKEN";
            }
        }

        std::cout << ")" << std::endl;
    }
}

bool player::save(const std::string &pFilename, bool pSilent) const {
    json j;
    j["money"] = mMoney;
    j["fuel"] = mFuel;
    j["inventory"] = json::array();

    for (const auto& item : mInventory) {
        json itemJson = {
            {"name", item.mName},
            {"type", item.mType},
            {"value", item.mValue},
            {"description", item.mDescription}
        };

        if (item.mType == "equipment") {
            itemJson["durability"] = item.mDurability;
            itemJson["isBroken"] = item.mIsBroken;
        }

        j["inventory"].push_back(itemJson);
    }

    j["museum"] = json::array();
    for (const auto& exhibit : mMuseumCollection) {
        j["museum"].push_back({
            {"name", exhibit.mName},
            {"type", exhibit.mType},
            {"value", exhibit.mValue},
            {"description", exhibit.mDescription}
        });
    }

    j["museumRewards"] = mMuseumRewards;

    std::ofstream file(pFilename);
    if (!file.is_open()) {
        std::cout << "ERROR: Could not open file " << pFilename << " for saving." << std::endl;
        return false;
    }

    file << j.dump(4);
    file.close();

    if (!pSilent) {
        std::cout << "Game saved successfully." << std::endl;
    }

    return true;
}

bool player::load(const std::string &pFilename) {
    std::ifstream file(pFilename);

    if (!file.is_open()) {
        std::cout << "No save file found, starting new game." << std::endl;
        return false;
    }

    try {
        json j;
        file >> j;
        file.close();

        const int loadedMoney = j.value("money", 500);
        const int loadedFuel = j.value("fuel", j.value("food", 10));
        std::vector<item> loadedInventory;

        for (const auto& item_index : j.value("inventory", json::array())) {
            item newItem(
                item_index.value("name", ""),
                item_index.value("type", ""),
                item_index.value("value", 0),
                item_index.value("description", "")
            );

            if (item_index.value("type", "") == "equipment") {
                newItem.mDurability = item_index.value("durability", 100);
                newItem.mIsBroken = item_index.value("isBroken", false);
            }

            loadedInventory.push_back(newItem);
        }

        std::vector<item> loadedMuseumCollection;
        for (const auto& exhibit : j.value("museum", json::array())) {
            loadedMuseumCollection.push_back(item(
                exhibit.value("name", ""),
                exhibit.value("type", ""),
                exhibit.value("value", 0),
                exhibit.value("description", "")
            ));
        }

        const std::vector<int> loadedMuseumRewards = j.value("museumRewards", std::vector<int>{});

        mMoney = loadedMoney;
        mFuel = loadedFuel;
        mInventory = std::move(loadedInventory);
        mMuseumCollection = std::move(loadedMuseumCollection);
        mMuseumRewards = loadedMuseumRewards;
    } catch (const json::exception& e) {
        std::cout << "Error loading save file: " << e.what() << std::endl;
        file.close();
        return false;
    }

    std::cout << "Game loaded successfully." << std::endl;
    return true;
}

void player::donate_to_museum(size_t pIndex) {
    if (pIndex >= mInventory.size() || mInventory[pIndex].mType != "loot") {
        std::cout << "Invalid item or not a loot item." << std::endl;
        return;
    }

    for (const auto& exhibit : mMuseumCollection) {
        if (exhibit.mName == mInventory[pIndex].mName) {
            std::cout << "This item is already in the museum!" << std::endl;
            return;
        }
    }

    item exhibit = mInventory[pIndex];

    if (exhibit.mName == "Podium Hat") {
        exhibit.mDescription = "A hat awarded to podium finishers.";
    } else if (exhibit.mName == "Engine Part") {
        exhibit.mDescription = "A high-performance engine part from a race.";
    } else if (exhibit.mName == "Tire Compound") {
        exhibit.mDescription = "A special tire compound used in high-speed races.";
    } else if (exhibit.mName == "Trophy Cap") {
        exhibit.mDescription = "A cap from a trophy ceremony.";
    } else if (exhibit.mName == "Suspension Part") {
        exhibit.mDescription = "A suspension component for better handling.";
    } else if (exhibit.mName == "Gearbox Component") {
        exhibit.mDescription = "A gearbox part for smooth shifting.";
    } else if (exhibit.mName == "Trophy Medal") {
        exhibit.mDescription = "A medal from a Grand Prix victory.";
    } else if (exhibit.mName == "Aero Wing Part") {
        exhibit.mDescription = "An aerodynamic wing for increased downforce.";
    } else if (exhibit.mName == "Brake System") {
        exhibit.mDescription = "A high-tech brake system for precise stopping.";
    }

    mMuseumCollection.push_back(exhibit);
    std::cout << "Donated " << exhibit.mName << " to the museum!" << std::endl;
    mInventory.erase(mInventory.begin() + pIndex);

    size_t exhibitCount = mMuseumCollection.size();
    if (exhibitCount >= 9 && std::find(mMuseumRewards.begin(), mMuseumRewards.end(), 9) == mMuseumRewards.end()) {
        add_money(300);
        mMuseumRewards.push_back(9);
        std::cout << "Congratulations! You collected 9 exhibits and earned a 300 money reward!" << std::endl;
    } else if (exhibitCount >= 6 && std::find(mMuseumRewards.begin(), mMuseumRewards.end(), 6) == mMuseumRewards.end()) {
        add_money(200);
        mMuseumRewards.push_back(6);
        std::cout << "Congratulations! You collected 6 exhibits and earned a 200 money reward!" << std::endl;
    } else if (exhibitCount >= 3 && std::find(mMuseumRewards.begin(), mMuseumRewards.end(), 3) == mMuseumRewards.end()) {
        add_money(100);
        mMuseumRewards.push_back(3);
        std::cout << "Congratulations! You collected 3 exhibits and earned a 100 money reward!" << std::endl;
    }
}

void player::show_museum() const {
    if (mMuseumCollection.empty()) {
        std::cout << "The museum is empty. Donate some loot to display!" << std::endl;
        return;
    }

    std::cout << "\n=== F1 Trophy Museum ===" << std::endl;
    std::cout << "Total exhibits: " << mMuseumCollection.size() << std::endl;

    for (const auto& exhibit : mMuseumCollection) {
        std::cout << "- " << exhibit.mName << std::endl;
        std::cout << "  Type: " << exhibit.mType << std::endl;
        std::cout << "  Value: " << exhibit.mValue << std::endl;

        if (!exhibit.mDescription.empty()) {
            std::cout << "  Description: " << exhibit.mDescription << std::endl;
        }
    }
}

bool player::buy_mod(const std::string &pModName, int pCost) {
    if (has_mod(pModName)) {
        std::cout << "You already have a working " << pModName << "!" << std::endl;
        return false;
    }

    if (get_money() < pCost) {
        std::cout << "Not enough money to buy " << pModName << "!" << std::endl;
        return false;
    }

    spend_money(pCost);
    add_item(create_default_equipment(pModName, pCost));
    std::cout << "Purchased " << pModName << " mod for " << pCost << " money." << std::endl;
    return true;
}

bool player::repair_equipment(size_t pIndex, int pCost) {
    if (pIndex >= mInventory.size() || mInventory[pIndex].mType != "equipment") {
        std::cout << "Invalid item or not a mod." << std::endl;
        return false;
    }

    if (!mInventory[pIndex].mIsBroken) {
        std::cout << mInventory[pIndex].mName << " mod is not broken." << std::endl;
        return false;
    }

    if (get_money() < pCost) {
        std::cout << "Not enough money to repair " << mInventory[pIndex].mName << "!" << std::endl;
        return false;
    }

    spend_money(pCost);
    mInventory[pIndex].mIsBroken = false;
    mInventory[pIndex].mDurability = kDefaultEquipmentDurability;
    std::cout << "Repaired " << mInventory[pIndex].mName << " mod for " << pCost << " money." << std::endl;

    return true;
}
