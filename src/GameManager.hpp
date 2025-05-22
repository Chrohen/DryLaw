#ifndef GAME_MANAGER
#define GAME_MANAGER

#include<memory>
#include<vector>
#include"District.hpp"
#include"Player.hpp"
#include"Mission.hpp"

class GameManager {
public:
	bool running = false;
	GameManager();
	void run();
private:
	std::vector<District> districts;
	std::vector<Mission> missions;
	std::unique_ptr<Player> player;

	void InitWorld();
	void InitMissions();
	void ShowMainMenu();
	void StartNewGame();
	void LoadGame();
	void ExitGame();

	void ShowInGameMenu();
	void HandleInGameInput(int input);

	void ShowTime() const;
};

#endif GAME_MANAGER