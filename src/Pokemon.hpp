#pragma once
#include <string>
#include <vector>
#include "LTexture.hpp"
#include "Attack.hpp"
#include "Enum.hpp"

class Pokemon
{
public:
    Pokemon();
    ~Pokemon();
    LTexture texture;
    LTexture textureBattle;
    LTexture nameText;
    LTexture hpText;
    LTexture manaText;

    std::vector<Attack> attacks;
    std::string name;
    int fullHp;
    int fullMana;
    int hp;
    int mana;
    int type = TYPE_NONE;

    bool isAlive = true;

    void init(std::string name, int hp, int mana, int type, SDL_Renderer *gRenderer, TTF_Font *font);
};