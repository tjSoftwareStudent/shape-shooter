#include <iostream>
#include <random>

#include "Systems/Game.h"

int main()
{
    srand(time(0));

    Game g("config.txt");
    g.run();
}