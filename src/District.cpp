#include "District.hpp"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include "GameClock.hpp"

using json = nlohmann::json;

District::District(const std::string& name) {
	this->name = name;
	
	std::ifstream ifs("./src/districts.json");
	if (!ifs.is_open()) {
		return;
	}

	json j;
	ifs >> j;
	

	for (const auto& entry : j) {
		if (entry.value("name", "") == name) {
			policeInfluence = entry.value("policeInfluence", 0);
			mafiaInfluence = entry.value("mafiaInfluence", 0);

			if (entry.contains("businesses") && entry["businesses"].is_array()) {
				for (const auto& list : entry["businesses"]) {
					InitBusiness(list.get<std::string>());
				}
			}
			return;
		}
	}
}

void District::VisitDistrict(Player& player) {
	while (true) {
		int idx = ShowBusinessList();
		if (idx < 0) break;

		businesses[idx]->interact(player);
	}
}

void District::InitBusiness(const std::string& name) {
	std::unique_ptr<Business> b = std::make_unique<Business>(name);
	businesses.push_back(std::move(b));
}

std::string District::GetName() {
	return name;
}

int District::GetInfluence(std::string faction) {
	if (faction == "Mafia") {
		return mafiaInfluence;
	}
	else if(faction == "Police")
	{
		return policeInfluence;
	}
	else {
		return 0;
	}
}

const std::vector<std::unique_ptr<Business>>& District::GetBusinesses() const {
	return businesses;
}

int District::ShowBusinessList() const {
	std::cout << "\n=== " << GameClock::toString() << " === " << name << " ===\n";
	if (businesses.empty()) {
		std::cout << "(No businesses in this district)\n";
		return -1;
	}
	for (size_t i = 0; i < businesses.size(); ++i) {
		std::cout << "[" << i + 1 << "] " << businesses[i]->GetName() << "\n";
	}
	std::cout << "[" << businesses.size() + 1 << "] Back\n";
	std::cout << "Choose : ";

	int choice;
	if (!(std::cin >> choice)) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return -1;
	}
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	if (choice < 1 || choice > static_cast<int>(businesses.size()) + 1) {
		std::cout << "Wrong choice.\n";
		return -1;
	}
	if (choice == static_cast<int>(businesses.size()) + 1) {
		return -1;
	}
	return choice - 1;
}