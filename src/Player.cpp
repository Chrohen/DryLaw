#include "Player.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include "GameClock.hpp"

using json = nlohmann::json;

Player::Player(const std::string& jsonPath) {
    std::ifstream ifs(jsonPath);
    if (!ifs.is_open()) {
        std::cerr << "[Player] File not found\n";
        hp = maxHp = 100;
        cleanMoney = dirtyMoney = 0;
        stats = { {"strength",5}, {"intelligence",5}, {"dexterity",5}, {"charisma",5} };
        maxStats = stats;
        relations = { {"mafia", 0}, {"police", 0}, {"politics", 0}, {"citizens", 0} };
        relationCap = 10;
        return;
    }

    json j;
    ifs >> j;

    hp = j.value("hp", 100);
    maxHp = j.value("maxHp", 100);
    cleanMoney = j.value("cleanMoney", 0);
    dirtyMoney = j.value("dirtyMoney", 0);

    stats = j["stats"].get<std::map<std::string, int>>();
    maxStats = j["maxStats"].get<std::map<std::string, int>>();
    relations = j["relations"].get<std::map<std::string, int>>();
    relationCap = j.value("relationCap", 10);
}

int Player::GetHP() const { return hp; }
int Player::GetCleanMoney() const { return cleanMoney; }
int Player::GetDirtyMoney() const { return dirtyMoney; }
int Player::GetStat(const std::string& stat) const {
    auto it = stats.find(stat);
    return it != stats.end() ? it->second : 0;
}
int Player::GetRelation(const std::string& faction) const {
    auto it = relations.find(faction);
    return it != relations.end() ? it->second : 0;
}
const std::map<std::string, int>& Player::GetRelations() const {
    return relations;
}

void Player::AddCleanMoney(int amount) {
    cleanMoney += amount;
}
void Player::AddDirtyMoney(int amount) {
    dirtyMoney += amount;
}

void Player::AddBusiness(std::shared_ptr<Business> b) {
    businesses.push_back(b);
}

void Player::Train() {
    std::cout << "\n=== " << GameClock::toString() << " === Training ===\n";
    int index = 1;
    std::vector<std::string> keys;
    for (const auto& [key, val] : stats) {
        std::cout << "[" << index << "] " << key << " (" << val << "/" << maxStats[key] << ")\n";
        keys.push_back(key);
        ++index;
    }
    std::cout << "Choose stat to train: ";
    int choice;
    std::cin >> choice;
    if (choice > 0 && choice <= (int)keys.size()) {
        auto& val = stats[keys[choice - 1]];
        if (val < maxStats[keys[choice - 1]]) {
            val++;
            std::cout << "You trained " << keys[choice - 1] << " to " << val << "\n";
        }
        else {
            std::cout << "That stat is already at max.\n";
        }
    }
}

void Player::ShowInfo() const {
    std::cout << "\n=== " << GameClock::toString() << " === Info ===\n";
    std::cout << "Hp: " << hp << "/" << maxHp << "\n";
    std::cout << "Clean money: " << cleanMoney << "\n";
    std::cout << "Dirty money: " << dirtyMoney << "\n";

    std::cout << "\nStats:\n";
    for (const auto& [k, v] : stats) {
        std::cout << k << ": " << v << "/" << maxStats.at(k) << "\n";
    }

    std::cout << "\nRelations:\n";
    for (const auto& [k, v] : relations) {
        std::cout << k << ": " << v << "\n";
    }

    std::cout << "\nBusinesses:\n";
    for (const auto& b : businesses) {
        std::cout << " - " << b->GetName() << "\n";
    }
}

void Player::GetProfit() {
    std::cout << "\n[Player] Recieving income:\n";
    for (const auto& b : businesses) {
        b->GiveProfit(*this);
    }
}
