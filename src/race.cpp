#include "race.h"

race::race(const std::string &pName, const std::string &pCircuitType, int pFee)
    : mName(pName), mCircuitType(pCircuitType), mFee(pFee) {}

std::string race::get_name() const {
    return mName;
}

std::string race::get_circuit_type() const {
    return mCircuitType;
}

int race::get_fee() const {
    return mFee;
}