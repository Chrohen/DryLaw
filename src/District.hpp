#ifndef DISTRICT
#define DISTRICT

#include <string>
#include <vector>
#include <memory>
#include "Business.hpp"

class District {
public:
    District(const std::string& name);

	std::string GetName();
	int GetInfluence(std::string faction);
	const std::vector<std::unique_ptr<Business>>& GetBusinesses() const;
	void VisitDistrict(Player& player);

private:
	std::string name;
    int policeInfluence = 0;
    int mafiaInfluence = 0;
	std::vector<std::unique_ptr<Business>> businesses;

	void InitBusiness(const std::string& name);

    int ShowBusinessList() const;
	int showBusinessActions() const;
};

#endif DISTRICT
