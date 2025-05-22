#include <iostream>
#include <fstream>
#include "GameManager.hpp"
#include "District.hpp"
#include "Player.hpp"
#include "nlohmann/json.hpp"
#include "GameClock.hpp"

using json = nlohmann::json;

GameManager::GameManager() {
}

void GameManager::InitWorld() {
	std::ifstream ifs("./src/districts.json");

	if (!ifs.is_open()) {
		std::cerr << "Ошибка: не удалось открыть districts.json\n";
		return;
	}

	json j;
	ifs >> j;

	for (const auto& districtJson : j) {
		std::string name = districtJson.at("name").get<std::string>();

		District d(name);

		districts.push_back(std::move(d));
	}
}

void GameManager::InitMissions()
{
	std::ifstream ifs("./src/missions.json");

	json jm; 
	ifs >> jm;
	for (auto& mj : jm) {
		missions.emplace_back(mj);
	}
}

void GameManager::run() {
	this->running = true;

	while (running) {
		ShowMainMenu();
		int choice;
		std::cin >> choice;
		std::cin.ignore(std::numeric_limits < std::streamsize>::max(), '\n');

		switch (choice) {
		case 1:
			StartNewGame();
			break;
		case 2: 
			LoadGame();
			break;
		case 3:
			ExitGame();
			break;
		default:
			std::cout << "Неверный выбору Попробуйте снова.\n";
		}
	}
}

void GameManager::ShowTime() const {
	std::cout << "[Time] " << GameClock::toString() << "\n";
}

void GameManager::ShowMainMenu() {
	std::cout << "\n=== Main Menu ===\n"
		<< "[1] New Game\n"
		<< "[2] Load Game\n"
		<< "[3] Exit\n"
		<< "Choose: ";
}

void GameManager::StartNewGame() {
	InitWorld();
	InitMissions();
	GameClock::hour = 8;
	player = std::make_unique<Player>("John");
	ShowInGameMenu();
}

void GameManager::LoadGame() {
	//TODO
}

void GameManager::ExitGame() {
	this->running = false;
}

void GameManager::ShowInGameMenu() {
	bool inGame = true;

	while (inGame) {
		std::cout << "\n=== " << GameClock::toString() << " === Actions Menu ===\n"
			<< "[1] Visit District\n"
			<< "[2] Missions\n"
			<< "[3] Training\n"
			<< "[4] Inspect your stats\n"
			<< "[5] Save\n"
			<< "[6] Exit to main menu\n"
			<< "Choose: ";

		int choice; 
		std::cin >> choice; std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		
		HandleInGameInput(choice);
	}
}

void GameManager::HandleInGameInput(int input) {
	switch (input)
	{
		int choice;
	case 1: //Выбор района для отправки
		std::cout << "\n=== " << GameClock::toString() << " === Districts ===\n";

		for (int i=1; i <= districts.size(); i++)
		{
			std::cout << "[" << i << "] " << districts[i-1].GetName() << "\n";
		}

		std::cout << "Choose: ";
		std::cin >> choice; std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		
		districts[choice- 1].VisitDistrict(*player);
		break;
	case 2: { // Просмотр миссий
		const auto& rels = player->GetRelations();
		std::vector<int> availableIdx;
		std::cout << "\n=== " << GameClock::toString() << " === Missions ===\n";
		int menuIdx = 1;
		for (int i = 0; i < (int)missions.size(); ++i) {
			if (missions[i].IsAvailable(rels)) {
				std::cout << "[" << menuIdx << "] " << missions[i].GetName() << "\n";
				availableIdx.push_back(i);
				++menuIdx;
			}
		}
		if (availableIdx.empty()) {
			std::cout << "No missions avaliable\n";
			break;
		}
		std::cout << "[" << menuIdx << "] Back\nChoose mission: ";
		int choice;
		std::cin >> choice;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		if (choice >= 1 && choice < menuIdx) {
			int idx = availableIdx[choice - 1];
			missions[idx].Execute(*player);
			GameClock::advance(missions[idx].GetTimeCost());
		}
		}
		break;
	case 3: // Тренировка
		player->Train();
		GameClock::advance(1);
		break;
	case 4: // Проверить статы
		player->ShowInfo();
		break;
	case 5: // Сокраниться
		break;
	case 6: // В главное меню
		break;
	default:
		std::cout << "Неверный выбору Попробуйте снова.\n";
		break;
	}
}