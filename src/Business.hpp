#ifndef BUSINESS_HPP
#define BUSINESS_HPP
#include <string>
#include <memory>
#include <nlohmann/json.hpp>
class Player;

class Business {
public:
    Business(const std::string& name);
    void ShowInfo() const;
    void GiveProfit(Player& owner) const;

    const std::string& GetName() const { return name; }
    bool IsLegal() const { return isLegal; }
    int GetPrice() const { return price; }

private:
    void LoadFromJson();

    std::string name;
    bool isLegal;
    int cleanIncome;
    int dirtyIncome;
    int dirtyToCleanConversion;
    int price;
    std::string owner;
};

#endif