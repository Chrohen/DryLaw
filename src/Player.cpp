#include "Player.hpp"
#include <fstream>
#include <iostream>
#include <limits>
#include <nlohmann/json.hpp>
#include "GameClock.hpp"

using json = nlohmann::json;

Player::Player(const std::string& playerName)
{
    name = playerName;

    std::ifstream ifs("./src/player.json");

    if (!ifs.is_open()) {
        hp = 100; 
        cleanMoney = 0;
        dirtyMoney = 0;
        return;
    }

    json j;
    ifs >> j;

    hp = j.value("hp", 100);
    maxHp = j.value("maxHp", 100);
    cleanMoney = j.value("cleanMoney", 0);
    dirtyMoney = j.value("dirtyMoney", 0);

    if (j.contains("stats") && j["stats"].is_object()) {
        for (auto& [key, val] : j["stats"].items()) {
            if (val.is_number_integer()) {
                stats[key] = val.get<int>();
            }
            else {
                std::cerr << "[Player] Пропущено stats[\"" << key << "\"] не целое\n";
            }
        }
    }
    else {
        std::cerr << "[Player] Поле \"stats\" отсутствует или не объект\n";
    }

    if (j.contains("maxStats") && j["maxStats"].is_object()) {
        for (auto& [key, val] : j["maxStats"].items()) {
            if (val.is_number_integer()) {
                maxStats[key] = val.get<int>();
            }
            else {
                std::cerr << "[Player] Пропущено maxStats[\"" << key << "\"] не целое\n";
            }
        }
    }
    else {
        std::cerr << "[Player] Поле \"maxStats\" отсутствует или не объект\n";
    }

    maxRelationAbs = j.value("maxRelationAbs", maxRelationAbs);

    if (j.contains("relations") && j["relations"].is_object()) {
        for (auto& [key, val] : j["relations"].items()) {
            if (val.is_number_integer()) {
                relations[key] = clampRelation(val.get<int>());
            }
        }
    }
}

int Player::GetHP() const { 
    return hp; 
}

int Player::GetCleanMoney() const {
    return cleanMoney; 
}

int Player::GetDirtyMoney() const { 
    return dirtyMoney; 
}

int Player::GetStat(const std::string& statName) const {
    auto it = stats.find(statName);
    if (it != stats.end()) return it->second;
    return 0;
}

int Player::GetRelation(const std::string& faction) const {
    auto it = relations.find(faction);
    return it != relations.end() ? it->second : 0;
}

const std::map<std::string, int>& Player::GetRelations() const {
    return relations;
}

void Player::ModifyRelation(const std::string& faction, int delta) {
    int cur = GetRelation(faction);
    int updated = clampRelation(cur + delta);
    relations[faction] = updated;
}

const std::string& Player::GetName() const { 
    return name; 
}


void Player::AddBusiness(std::shared_ptr<Business> business) {
    ownedBusinesses.push_back(business);
}

void Player::GetProfit() {
    for (const auto& b : ownedBusinesses) {
        auto [clean, dirty] = b->CalculateIncome();
        cleanMoney += clean;
        dirtyMoney += dirty;

        b->ConvertDirtyMoney(*this);
    }
}

void Player::Train() {
    std::cout << "\n=== " << GameClock::toString() << " === Training ===\n";

    int idx = 1;

    for (auto& [key, val] : stats) {
        std::cout << "[" << idx << "] " << key << " (current: " << val << ")\n";
        idx++;
    }

    int choice;

}

void Player::ModifyCleanMoney(int delta) {
    cleanMoney += delta;
    if (cleanMoney < 0) cleanMoney = 0;
}
void Player::ModifyDirtyMoney(int delta) {
    dirtyMoney += delta;
    if (dirtyMoney < 0) dirtyMoney = 0;
}

void Player::ShowInfo() const {
    std::cout << "\n=== Info ===\n";
    std::cout << "Hp: " << hp << "/" << maxHp << "\n";
    std::cout << "Clean money: " << cleanMoney << "\n";
    std::cout << "Dirty money: " << dirtyMoney << "\n\n";

    std::cout << "Stats:\n";
    for (const auto& [key, val] : stats) {
        int maxV = maxStats.count(key) ? maxStats.at(key) : val;
        std::cout << key << ": " << val << "/" << maxV << "\n";
    }
    std::cout << "\n";

    std::cout << "\nRelations\n";
    for (auto& [key, val] : relations) {
        std::cout << key << ": " << val << "\n";
    }

    std::cout << "\nBusinesses:\n";
    if (ownedBusinesses.empty()) {
        std::cout << "  (No)\n";
    }
    else {
        for (const auto& b : ownedBusinesses) {
            std::cout << "  - " << b->GetName() << "\n";
        }
    }
    std::cout << std::endl;

    int choice;
    std::cout << "[Any] Back\n";
    std::cin >> choice;
}

int Player::clampRelation(int value) const {
    if (value > maxRelationAbs)   return maxRelationAbs;
    if (value < -maxRelationAbs)  return -maxRelationAbs;
    return value;
}