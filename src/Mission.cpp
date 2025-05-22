#include "Mission.hpp"
#include "Player.hpp"
#include <iostream>
#include <cstdlib>
#include "GameClock.hpp"

Mission::Mission(const nlohmann::json& j)
    : name(j.value("name", "")),
    legal(j.value("legal", false)),
    skillCheck(j.value("skillCheck", "")),
    difficulty(j.value("difficulty", 0)),
    timeCost(j.value("timeCost", 1)),
    reward(j.value("reward", 0)) {

    if (j.contains("delta")) {
        delta = j["delta"].get<std::map<std::string, int>>();
    }
    if (j.contains("requirements")) {
        requirements = j["requirements"].get<std::map<std::string, int>>();
    }
}

const std::string& Mission::GetName() const {
    return name;
}

bool Mission::IsLegal() const {
    return legal;
}

const std::string& Mission::GetSkillCheck() const {
    return skillCheck;
}

int Mission::GetDifficulty() const {
    return difficulty;
}

int Mission::GetTimeCost() const {
    return timeCost;
}

int Mission::GetReward() const {
    return reward;
}

bool Mission::Attempt(int playerStat) const {
    int roll = rand() % 20 + 1 + playerStat;
    return roll >= difficulty;
}

void Mission::Describe() const {
    std::cout << "\n=== " << GameClock::toString() << " === "<< name << " ===\n";
    std::cout << "Legal: " << (legal ? "$" : "d$") << "\n";
    std::cout << "Skillcheck: " << skillCheck << "\n";
    std::cout << "Difficulty: " << difficulty << "\n";
    std::cout << "Reward: " << reward << (legal ? " $" : " d$") << "\n";
    std::cout << "Relations change:\n";
    for (const auto& [k, v] : delta) {
        std::cout << "  " << k << ": " << (v >= 0 ? "+" : "") << v << "\n";
    }
    std::cout << "[1] Attempt\n[2] Back\n";
}

bool Mission::IsAvailable(const std::map<std::string, int>& playerRelations) const {
    for (const auto& [faction, required] : requirements) {
        auto it = playerRelations.find(faction);
        if (it == playerRelations.end() || it->second < required) return false;
    }
    return true;
}

void Mission::Execute(Player& player) const {
    Describe();
    int choice;
    std::cin >> choice;
    if (choice != 1) return;

    int stat = player.GetStat(skillCheck);
    bool success = Attempt(stat);
    std::cout << (success ? "[Success] Mission completed!\n" : "[Fail] Mission failed!\n");

    if (success) {
        if (legal) player.AddCleanMoney(reward);
        else player.AddDirtyMoney(reward);
    }

    for (const auto& [faction, value] : delta) {
        int deltaVal = success ? value : -std::abs(value);
        std::cout << "Relations with " << faction << (deltaVal >= 0 ? " increased " : " decreased ") << std::abs(deltaVal) << "\n";
        // TODO: реализовать обновление отношений у Player
    }
}
