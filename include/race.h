#ifndef RACE_H
#define RACE_H

#include "player.h"
#include "probabilities.h"
#include <string>
#include <random>

class race {
protected:
    std::string mName;
    std::string mCircuitType;
    int mFee;
    std::mt19937 mRandomNumberGenerator;

public:
    race(const std::string& pName, const std::string& pCircuitType, int pFee);
    virtual ~race() = default;

    virtual void drive(player& pPlayer, const std::string& pMod) = 0;
    virtual void effect(player& pPlayer) = 0;
    virtual void print_text() const = 0;

    [[nodiscard]] std::string get_name() const;
    [[nodiscard]] std::string get_circuit_type() const;
    [[nodiscard]] int get_fee() const;
};

#endif // RACE_H