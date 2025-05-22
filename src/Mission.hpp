#ifndef MISSION
#define MISSION

#include <string>
#include <map>
#include <nlohmann/json.hpp>
#include "Player.hpp" 

class Mission {
public:
    explicit Mission(const nlohmann::json& j);

    const std::string& GetName() const;
    bool IsLegal() const;
    const std::string& GetSkillCheck() const;
    int GetDifficulty() const;
    int GetTimeCost() const;
    int GetReward() const;

    // Теперь возвращает map фракция→дельта
    const std::map<std::string, int>& GetDelta() const;

    bool Attempt(int skillValue) const;
    bool IsAvailable(const std::map<std::string, int>& playerRelations) const;
    void Describe() const;
    void Execute(Player& player);

private:
    std::string name;
    bool legal;
    std::string skillCheck;
    int difficulty;
    int timeCost;
    int reward;

    std::map<std::string, int> delta;         // Mafia, Police, Politics, Citizens
    std::map<std::string, int> requirements;  // требования по отношениям
};

#endif // !MISSION
