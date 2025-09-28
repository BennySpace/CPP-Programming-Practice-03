#ifndef RACE_H
#define RACE_H

#include "player.h"
#include <string>
#include <random>

class race {
protected:
    std::string mName;
    std::string mCircuitType;
    int mCost;
    std::mt19937 mRandomNumberGenerator;

public:
    race(const std::string& pName, const std::string& pCircuitType, int pCost);
    virtual ~race() = default;

    virtual void drive(player& pPlayer, const std::string& pMod) = 0;
    virtual void effect(player& pPlayer) = 0;
    virtual void print_text() const = 0;

    std::string get_name() const;
    std::string get_circuit_type() const;
    int get_cost() const;
};

#endif // RACE_H