#ifndef RACE_SPA_H
#define RACE_SPA_H

#include "race.h"

class race_spa final : public race {
public:
    race_spa();

    void drive(player& pPlayer, const std::string &pMod) override;
    void effect(player& pPlayer) override;
    void print_text() const override;
};

#endif // RACE_SPA_H
