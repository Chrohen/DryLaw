#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Business.hpp"

class Player {
public:
    Player(const std::string& jsonPath);

    int GetHP() const;
    int GetCleanMoney() const;
    int GetDirtyMoney() const;
    int GetStat(const std::string& stat) const;
    int GetRelation(const std::string& faction) const;

    void AddCleanMoney(int amount);
    void AddDirtyMoney(int amount);
    void Train();
    void ShowInfo() const;
    void GetProfit();

    void AddBusiness(std::shared_ptr<Business> business);
    const std::map<std::string, int>& GetRelations() const;

private:
    std::string name;
    int hp;
    int maxHp;
    int cleanMoney;
    int dirtyMoney;

    std::map<std::string, int> stats;
    std::map<std::string, int> maxStats;
    std::map<std::string, int> relations;
    int relationCap;

    std::vector<std::shared_ptr<Business>> businesses;
};

#endif