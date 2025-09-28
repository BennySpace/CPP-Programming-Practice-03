#ifndef RACE_MONZA_H
#define RACE_MONZA_H

#include "race.h"

class race_monza : public race {
public:
    race_monza();

    void drive(player& pPlayer, const std::string& pMod) override;
    void effect(player& pPlayer) override;
    void print_text() const override;
};

#endif // RACE_MONZA_H