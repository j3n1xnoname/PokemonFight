#pragma once
#include "Pokemon.hpp"
#include <vector>

class Player
{
public:
    Player();
    ~Player();
    std::vector<Pokemon> playerPokemons;
    bool isAlive = true;

    int countAlivePokemons = 3;
};