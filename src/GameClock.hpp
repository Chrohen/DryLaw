#ifndef GAME_CLOCK
#define GAME_CLOCK

#include <string>

class GameClock {
public:
    static int hour;

    static bool advance(int h);

    static std::string toString();
};

#endif // GAME_CLOCK