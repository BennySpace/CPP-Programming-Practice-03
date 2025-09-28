#ifndef RACE_MONACO_H
#define RACE_MONACO_H

#include "race.h"

class race_monaco : public race {
public:
    race_monaco();

    void drive(player& pPlayer, const std::string& pMod) override;
    void effect(player& pPlayer) override;
    void print_text() const override;
};

#endif // RACE_MONACO_H