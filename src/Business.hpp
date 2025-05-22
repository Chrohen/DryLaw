#ifndef BUSINESS
#define BUSINESS

#include <string>

class Player;

class Business {
public:
    // ����������� ���� ������ �� ����� � businesses.json
    explicit Business(const std::string& name);

    // �������
    const std::string& GetName() const;
    bool IsLegal() const;
    int GetCleanIncome() const;
    int GetDirtyIncome() const;
    int GetConversion() const;

    // ������ ������� (������, �������)
    std::pair<int, int> CalculateIncome() const;
    void ConvertDirtyMoney(Player& player) const;

    void interact(Player& player);

private:
    std::string name;
    bool legal = true;
    int cleanIncome = 0;
    int dirtyIncome = 0;
    int dirtyToCleanConversion = 0;

    static std::string factionToString();
    void describe() const;
    int  chooseAction() const;
    void handleAction(int action, Player& player) const;
};

#endif // !BUISNESS
