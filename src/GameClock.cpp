#include "GameClock.hpp"
#include <iomanip>
#include <sstream>

int GameClock::hour = 8;

bool GameClock::advance(int h) {
    int prev = hour;
    int next = (hour + h) % 24;
    if (next < 0) next += 24;

    hour = next;

    if ((prev + h) >= 24 || (prev + h) < 0) {
        return true;
    }
    return false;
}

std::string GameClock::toString() {
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << hour << ":00 ";
    if (hour >= 6 && hour < 12)        oss << "(morning)";
    else if (hour >= 12 && hour < 18)  oss << "(afternoon)";
    else if (hour >= 18 && hour < 22)  oss << "(evening)";
    else                                oss << "(night)";
    return oss.str();
}
