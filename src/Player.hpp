#pragma once
#include "Pokemon.hpp"
#include <vector>

class Player
{
public:
    Player();
    ~Player();
    std::vector<Pokemon> playerPokemons;
    int pokemonIndex = INDEX_POKEMON_ONE;
    bool isAlive = true;

    int countAlivePokemons = 3;
};