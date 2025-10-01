#include "game.h"
#include <iostream>

int main() {
    std::cout << "Akira Ayanami Prod. proudly presents..." << std::endl;
    std::cout << R"(
  _____   _     ____  _                 _       _
 |  ___| / |   / ___|(_)_ __ ___  _   _| | __ _| |_ ___  _ __
 | |_    | |   \___ \| | '_ ` _ \| | | | |/ _` | __/ _ \| '__|
 |  _|   | |    ___) | | | | | | | |_| | | (_| | || (_) | |
 |_|     |_|   |____/|_|_| |_| |_|\__,_|_|\__,_|\__\___/|_|
    )" << std::endl;
    std::cout << "\nPress Enter to continue.";
    std::cin.get();

    game game;
    game.run();

    return 0;
}