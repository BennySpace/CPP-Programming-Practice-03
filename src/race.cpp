#include "race.h"

race::race(const std::string &pName, const std::string &pCircuitType, int pCost)
    : mName(pName), mCircuitType(pCircuitType), mCost(pCost) {}

std::string race::get_name() const {
    return mName;
}

std::string race::get_circuit_type() const {
    return mCircuitType;
}

int race::get_cost() const {
    return mCost;
}