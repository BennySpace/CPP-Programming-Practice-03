#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "race.h"
#include <string>
#include <vector>

class game {
private:
    player mPlayer;
    std::vector<race*> mRaces;
    const std::string mSaveFile = SAVE_FILE_PATH;

public:
    game();
    ~game();

    void run();

private:
    [[nodiscard]] bool can_player_continue() const;
    void show_main_menu();
    void handle_main_menu(int pChoice);
    void choose_race();
    void start_race(race* pRace);
    void visit_shop();
    void visit_museum();
};

#endif // GAME_H
