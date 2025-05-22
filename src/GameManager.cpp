#include "GameManager.hpp"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "GameClock.hpp"

using json = nlohmann::json;

GameManager::GameManager() : running(false) {
    GameClock::hour = 8;
}

void GameManager::Run() {
    running = true;
    while (running) {
        ShowMainMenu();
        int choice;
        std::cin >> choice;
        switch (choice) {
        case 1: StartNewGame(); break;
        case 2: LoadGame(); break;
        case 3: ExitGame(); break;
        default: std::cout << "Wrong choice\n"; break;
        }
    }
}

void GameManager::ShowMainMenu() {
    std::cout << "\n=== Main menu ===\n";
    std::cout << "[1] New Game\n[2] Load Game\n[3] Exit\nChoose: ";
}

void GameManager::StartNewGame() {
    player = std::make_unique<Player>("src/player.json");
    InitWorld();
    InitMissions();
    ShowInGameMenu();
}

void GameManager::LoadGame() {
}

void GameManager::ExitGame() {
    running = false;
}

void GameManager::InitWorld() {
    std::ifstream ifs("src/districts.json");

    if (!ifs.is_open()) {
        std::cerr << "[InitWorld] Cannot open districts.json\n";
        return;
    }

    json j;
    ifs >> j;

    for (const auto& d : j) {
        districts.emplace_back(d["name"].get<std::string>());
    }
}

void GameManager::InitMissions() {
    std::ifstream ifs("src/missions.json");
    if (!ifs.is_open()) {
        std::cerr << "[InitMissions] Cannot open missions.json\n";
        return;
    }
    json j;
    ifs >> j;
    for (const auto& m : j) {
        missions.emplace_back(m);
    }
}

void GameManager::ShowInGameMenu() {
    while (true) {
        std::cout << "\n=== "<< GameClock::toString() << " === Actions ===\n";
        std::cout << "[1] Visit District\n[2] Missions\n[3] Training\n[4] Statistics\n[5] Back\nChoose: ";
        int choice;
        std::cin >> choice;
        if (choice == 5) break;
        HandleInGameInput(choice);
    }
}

void GameManager::HandleInGameInput(int input) {
    switch (input) {
    case 1:
        std::cout << "\n=== " << GameClock::toString() << " ===  Districts ===\n";
        for (size_t i = 0; i < districts.size(); ++i) {
            std::cout << "[" << i + 1 << "] " << districts[i].GetName() << "\n";
        }
        std::cout << "Choose: ";
        int choice;
        std::cin >> choice;
        if (choice > 0 && choice <= (int)districts.size()) {
            districts[choice - 1].VisitDistrict(*player);
        }
        break;
    case 2:
        std::cout << "\n=== " << GameClock::toString() << " === Missions ===\n";
        for (size_t i = 0; i < missions.size(); ++i) {
            if (missions[i].IsAvailable(player->GetRelations())) {
                std::cout << "[" << i + 1 << "] " << missions[i].GetName() << "\n";
            }
        }
        std::cout << "Choose: ";
        std::cin >> choice;
        if (choice > 0 && choice <= (int)missions.size()) {
            missions[choice - 1].Execute(*player);
        }
        break;
    case 3:
        player->Train();
        break;
    case 4:
        player->ShowInfo();
        break;
    default:
        std::cout << "Wrong choice\n";
    }
}
