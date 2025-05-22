#include "Business.hpp"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "Player.hpp" 
using json = nlohmann::json;

Business::Business(const std::string& name) : name(name) {
    std::ifstream ifs("./src/buisness.json");

    json j;
    ifs >> j;

    for (const auto& entry : j) {
        if (entry.contains("name") && entry["name"].is_string() && entry["name"] == name) {
            legal = entry.value("isLegal", true);
            cleanIncome = entry.value("cleanIncome", 0);
            dirtyIncome = entry.value("dirtyIncome", 0);
            dirtyToCleanConversion = entry.value("dirtyToCleanConversion", 0);

            return;
        }
    }
}

const std::string& Business::GetName() const {
    return name;
}

bool Business::IsLegal() const {
    return legal;
}

int Business::GetCleanIncome() const {
    return cleanIncome;
}

int Business::GetDirtyIncome() const {
    return dirtyIncome;
}

int Business::GetConversion() const {
    return dirtyToCleanConversion;
}

std::pair<int, int> Business::CalculateIncome() const {
    if (legal) {
        return { cleanIncome, 0 };
    }
    else {
        return { 0, dirtyIncome };
    }
}

void Business::ConvertDirtyMoney(Player& player) const {
    if (!legal || dirtyToCleanConversion <= 0) return;

    int playerDirty = player.GetDirtyMoney();
    int canConvert = std::min(dirtyToCleanConversion, playerDirty);

    if (canConvert > 0) {
        player.ModifyDirtyMoney(-canConvert);
        player.ModifyCleanMoney(+canConvert);
    }
}

void Business::describe() const {
    std::cout << "\n== " << name << " ==\n";
    // std::cout << "Цена: " << price << " $\n";
    if (legal) {
        std::cout << "Чистый доход: " << cleanIncome << " $\n";
        if (dirtyToCleanConversion > 0) {
            std::cout << "Затраты на отмывание: " << dirtyToCleanConversion << " d$\n";
        }
    }
    else {
        std::cout << "Грязный доход: " << dirtyIncome << " d$\n";
    }
}

int Business::chooseAction() const {
    std::cout << "\n[1] Купить\n"
        << "[2] Ограбить\n"
        << "[3] Рейд\n"
        << "[4] Посетить\n\n"
        << "[5] Назад\n"
        << "Выберите действие: ";
    int action;
    if (!(std::cin >> action)) {
        std::cin.clear();
        action = 5;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (action < 1 || action > 5) action = 5;
    return action;
}

void Business::handleAction(int action, Player& player) const {
    switch (action) {
    case 1:
        std::cout << ">> Покупка \"" << name << "\" (TODO)\n";
        break;
    case 2:
        std::cout << ">> Ограбление \"" << name << "\" (TODO)\n";
        break;
    case 3:
        std::cout << ">> Рейд на \"" << name << "\" (TODO)\n";
        break;
    case 4:
        std::cout << ">> Посещение \"" << name << "\" (TODO)\n";
        break;
    default:
        break;
    }
}

void Business::interact(Player& player) {
    describe();
    int action = chooseAction();
    if (action == 5) return;
    handleAction(action, player);
}