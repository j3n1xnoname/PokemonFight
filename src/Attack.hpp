#pragma once
#include <string>
#include "LTexture.hpp"

class Attack
{
public:
    Attack();
    ~Attack();
    void init(int attackDamage, std::string attackName, std::string attackDescription, int attackWasteMana, SDL_Renderer *gRenderer, TTF_Font *gFontGame);

    LTexture attackDamageText;
    LTexture attackNameText;
    LTexture attackWasteManaText;
    LTexture attackDescriptionText;

    int attackDamage;
    std::string attackName;
    std::string attackDescription;
    int attackWasteMana;
};