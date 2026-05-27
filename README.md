# ITMO.GameDev / C++ Programming / Practice 03 - F1 Simulator

## Project Description

This project is a C++ console game where the player manages an F1 team headquarters, enters Grand Prix events, upgrades car equipment, collects loot, and donates trophies to a museum collection.

The application supports the following operations:
1. **Enter Grand Prix events**: Choose one of several race tracks with different entry fees and random effects.
2. **Manage resources**: Spend money on race entry, buy fuel, and keep the team operational.
3. **Buy and repair equipment**: Maintain car modifications such as aerodynamics, engine, and tires.
4. **Collect and sell loot**: Receive race-related items and sell them in the shop for money.
5. **Build a museum collection**: Donate unique loot items to the museum and unlock milestone rewards.
6. **Save progress**: Store player progress in the project-root `savegame.json` so the game can be continued later.

## Build Instructions

1. Ensure CMake and a C++17-compatible compiler (e.g., g++) are installed.
2. Navigate to the project root directory.
3. Create and enter a build directory:
   ```bash
   mkdir build
   cd build
   ```
4. Run CMake and build the project:
   ```bash
   cmake ..
   cmake --build .
   ```
5. Run the executable:
   ```bash
   ./out/build/x64-Debug/FormulaOneSimulator
   ```

## Usage

Run the program and select actions from the main menu:
- `1`: Enter a Grand Prix.
- `2`: Visit the Team Shop.
- `3`: Visit the F1 Museum.
- `0`: Exit the game.

During the game, you can:
- Choose a race track and pay the entry fee.
- Use available car modifications during races.
- Buy fuel and equipment in the shop.
- Sell collected loot items.
- Repair broken equipment mods.
- Donate loot items to the museum collection.

## Dependencies

This project uses the following third-party libraries:

- [nlohmann/json](https://github.com/nlohmann/json) - A modern C++ JSON library (MIT License).
