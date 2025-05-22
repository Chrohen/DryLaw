#ifndef MISSION_HPP
#define MISSION_HPP

#include <string>
#include <map>
#include <nlohmann/json.hpp>
class Player;

class Mission {
public:
    Mission(const nlohmann::json& j);

    const std::string& GetName() const;
    bool IsLegal() const;
    const std::string& GetSkillCheck() const;
    int GetDifficulty() const;
    int GetTimeCost() const;
    int GetReward() const;

    bool Attempt(int playerStat) const;
    void Describe() const;
    bool IsAvailable(const std::map<std::string, int>& playerRelations) const;
    void Execute(Player& player) const;

private:
    std::string name;
    bool legal;
    std::string skillCheck;
    int difficulty;
    int timeCost;
    int reward;
    std::map<std::string, int> delta;
    std::map<std::string, int> requirements;
};

#endif