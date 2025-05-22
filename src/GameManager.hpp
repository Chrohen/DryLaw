// GameManager.hpp
#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <vector>
#include <memory>
#include "Player.hpp"
#include "District.hpp"
#include "Mission.hpp"

class GameManager {
public:
    GameManager();
    void Run();

private:
    void InitWorld();
    void InitMissions();
    void ShowMainMenu();
    void StartNewGame();
    void LoadGame();
    void ExitGame();
    void ShowInGameMenu();
    void HandleInGameInput(int input);

    std::unique_ptr<Player> player;
    std::vector<District> districts;
    std::vector<Mission> missions;
    bool running;
};

#endif