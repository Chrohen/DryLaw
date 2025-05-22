#ifndef DISTRICT_HPP
#define DISTRICT_HPP

#include <string>
#include <vector>
#include <memory>
#include "Business.hpp"
class Player;

class District {
public:
    District(const std::string& name);
    void VisitDistrict(Player& player);
    const std::string& GetName() const;

private:
    void ShowBusinesses() const;
    void InteractWithBusiness(Player& player, int index);

    std::string name;
    int policeInfluence;
    int mafiaInfluence;
    std::vector<std::shared_ptr<Business>> businesses;
};

#endif