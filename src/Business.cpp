#include "Business.hpp"
#include "Player.hpp"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

Business::Business(const std::string& businessName) : name(businessName) {
    LoadFromJson();
}

void Business::LoadFromJson() {
    std::ifstream ifs("src/businesses.json");

    if (!ifs.is_open()) {
        throw std::runtime_error("Cannot open businesses.json");
    }

    nlohmann::json data = nlohmann::json::parse(ifs);


    for (const auto& entry : data) {
        if (entry["name"] == name) {
            isLegal = entry.value("isLegal", true);
            cleanIncome = entry.value("cleanIncome", 0);
            dirtyIncome = entry.value("dirtyIncome", 0);
            dirtyToCleanConversion = entry.value("dirtyToCleanConversion", 0);
            price = entry.value("price", 1000);
            owner = entry.value("owner", "Mafia");
            return;
        }
    }
    throw std::runtime_error("Business not found in JSON: " + name);
}

void Business::ShowInfo() const {
    std::cout << "== " << name << " ==\n";
    std::cout << "Owner: " << owner << "\n";
    std::cout << "Price: " << price << " $\n";

    if (isLegal) {
        std::cout << "Clean income: " << cleanIncome << " $\n";
        if (dirtyToCleanConversion > 0) {
            std::cout << "Clean up expensses: " << dirtyToCleanConversion << " d$\n";
            std::cout << "Clean up income: " << dirtyToCleanConversion << " $\n";
        }
    }
    else {
        std::cout << "Dirty income: " << dirtyIncome << " d$\n";
    }
}

void Business::GiveProfit(Player& ownerRef) const {
    if (isLegal) {
        ownerRef.AddCleanMoney(cleanIncome);
        if (dirtyToCleanConversion > 0 && ownerRef.GetDirtyMoney() >= dirtyToCleanConversion) {
            ownerRef.AddDirtyMoney(-dirtyToCleanConversion);
            ownerRef.AddCleanMoney(dirtyToCleanConversion);
        }
    }
    else {
        ownerRef.AddDirtyMoney(dirtyIncome);
    }
}
