#include "Game.hpp"

constexpr int SCREEN_WIDTH = 480;
constexpr int SCREEN_HEIGHT = 256;

int main()
{
    Game game { SCREEN_WIDTH, SCREEN_HEIGHT };

    while ( game.update() )
    {}

    return 0;
}