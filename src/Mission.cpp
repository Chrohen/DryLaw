// Mission.cpp
#include "Mission.hpp"
#include <iostream>
#include <cstdlib>
#include <limits>
#include "Player.hpp"
#include "GameClock.hpp"

using json = nlohmann::json;

Mission::Mission(const json& j)
    : name(j.value("name", "")),
    legal(j.value("legal", false)),
    skillCheck(j.value("skillCheck", "")),
    difficulty(j.value("difficulty", 0)),
    timeCost(j.value("timeCost", 0)),
    reward(j.value("reward", 0))
{
    if (j.contains("delta") && j["delta"].is_object()) {
        for (const auto& [faction, v] : j["delta"].items()) {
            if (v.is_number_integer()) {
                delta[faction] = v.get<int>();
            }
        }
    }

    if (j.contains("requirements") && j["requirements"].is_object()) {
        for (const auto& [faction, v] : j["requirements"].items()) {
            if (v.is_number_integer()) {
                requirements[faction] = v.get<int>();
            }
        }
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

const std::map<std::string, int>& Mission::GetDelta() const {
    return delta;
}

bool Mission::Attempt(int skillValue) const {
    int roll = (std::rand() % 20 + 1) + skillValue;
    return roll >= difficulty;
}

bool Mission::IsAvailable(const std::map<std::string, int>& playerRelations) const {
    for (const auto& [faction, req] : requirements) {
        auto it = playerRelations.find(faction);
        if (it == playerRelations.end() || it->second < req)
            return false;
    }
    return true;
}

void Mission::Describe() const {
    std::cout << "\n=== " << GameClock::toString() << " === " << name << " ===\n"
        << "Is it legal: " << (legal ? "Legal" : "Illigal") << "\n"
        << "Skill check: " << skillCheck << "\n"
        << "Relations change:\n";

    for (const auto& [faction, d] : delta) {
        std::cout << "  " << faction << ": "
            << (d >= 0 ? "+" : "") << d << "\n";
    }

    std::cout << "Reward: " << reward << (legal ? " $" : " d$") << "\n";
}

void Mission::Execute(Player& player) {
    // 1) Описание миссии
    Describe();
    std::cout << "\n[1] Attempt\n[2] Back\nChoose: ";

    int action;
    std::cin >> action;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (action != 1) {
        return;
    }

    int playerStat = player.GetStat(skillCheck);
    bool success = Attempt(playerStat);

    std::cout << "Time spent: " << timeCost << "\n";

    if (success) {
        std::cout << "Success! Reward: " << reward
            << (legal ? " $" : " d$") << "\n";
        if (legal) player.ModifyCleanMoney(reward);
        else      player.ModifyDirtyMoney(reward);

        for (auto& [faction, d] : delta) {
            player.ModifyRelation(faction, d);
            std::cout << faction << ": "
                << (d >= 0 ? "+" : "") << d << "\n";
        }
    }
    else {
        std::cout << "Fail!\n";
        for (auto& [faction, d] : delta) {
            int penal = d > 0 ? -d : d;
            player.ModifyRelation(faction, penal);
            std::cout << faction << ": "
                << (penal >= 0 ? "+" : "") << penal << "\n";
        }
    }
}
