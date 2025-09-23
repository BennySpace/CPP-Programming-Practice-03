#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Excavation.h"
#include <vector>

class Game {
public:
    Game();
    ~Game();

    Player player;
    std::vector<Excavation*> expeditions;

    void run();

private:
    void showMainMenu();
    void handleMainMenu(int choice);
    void chooseExpedition();
    void startExpedition(Excavation* expedition);
    void visitShop();
    void visitMuseum();
};

#endif // GAME_H