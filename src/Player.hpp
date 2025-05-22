#ifndef PLAYER
#define PLAYER

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "Business.hpp"

class Player {
public:
    Player(const std::string& playerName);

    int GetHP() const;
    int GetCleanMoney() const;
    int GetDirtyMoney() const;

    int GetStat(const std::string& statName) const;
    int GetRelation(const std::string& faction) const;
    const std::map<std::string, int>& GetRelations() const;

    const std::string& GetName() const;

    void Train();

    void GetProfit();
    void AddBusiness(std::shared_ptr<Business> business);
    void ModifyCleanMoney(int delta);
    void ModifyDirtyMoney(int delta);

    void ShowInfo() const;
    void ModifyRelation(const std::string& faction, int delta);

private:
    std::string name;

    int hp;
    int maxHp;
    int cleanMoney;
    int dirtyMoney;

    std::map<std::string,int> stats;
    std::map<std::string,int> maxStats;

    std::map<std::string, int> relations;
    int maxRelationAbs;

    int clampRelation(int value) const;

    std::vector<std::shared_ptr<Business>> ownedBusinesses;
};

#endif // !PLAYER