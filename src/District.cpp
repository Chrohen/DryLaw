#include "District.hpp"
#include "Player.hpp"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

District::District(const std::string& districtName) : name(districtName) {
    std::ifstream ifs("src/districts.json");;

    if (!ifs.is_open()) {
        std::cerr << "[District] Cannot open districts.json\n";
        return;
    }

    json j;
    ifs >> j;

    for (const auto& entry : j) {
        if (entry["name"] == name) {
            policeInfluence = entry.value("policeInfluence", 0);
            mafiaInfluence = entry.value("mafiaInfluence", 0);

            if (entry.contains("businesses") && entry["businesses"].is_array()) {
                for (const auto& bizName : entry["businesses"]) {
                    businesses.push_back(std::make_shared<Business>(bizName.get<std::string>()));
                }
            }
            return;
        }
    }

    std::cerr << "[District] District with '" << name << "' not found in JSON\n";
}

const std::string& District::GetName() const {
    return name;
}

void District::VisitDistrict(Player& player) {
    std::cout << "\n=== District: " << name << " ===\n";
    ShowBusinesses();
    std::cout << "[0] Back\nChoose business: ";
    int choice;
    std::cin >> choice;
    if (choice > 0 && choice <= (int)businesses.size()) {
        InteractWithBusiness(player, choice - 1);
    }
}

void District::ShowBusinesses() const {
    int index = 1;
    for (const auto& b : businesses) {
        std::cout << "[" << index++ << "] " << b->GetName() << "\n";
    }
}

void District::InteractWithBusiness(Player& player, int index) {
    if (index < 0 || index >= (int)businesses.size()) return;
    auto& biz = businesses[index];
    biz->ShowInfo();

    std::cout << "\n[1] Buy\n[2] Back\nChoose: ";
    int act;
    std::cin >> act;

    if (act == 1 && player.GetCleanMoney() >= biz->GetPrice()) {
        player.AddBusiness(biz);
        player.AddCleanMoney(-biz->GetPrice());
        std::cout << "[INFO] Business baught!\n";
    }
    else if (act == 1) {
        std::cout << "[INFO] Not enough money.\n";
    }
}
