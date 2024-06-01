#include "Attack.hpp"

Attack::Attack()
{
    attackDamage = 0;
    attackName = "";
    attackDescription = "";
    attackWasteMana = 0;
}

void Attack::init(int attackDamage, std::string attackName, std::string attackDescription, int attackWasteMana, SDL_Renderer *gRenderer, TTF_Font *gFontGame)
{
    this->attackDamage = attackDamage;
    this->attackName = attackName;
    this->attackDescription = attackDescription;
    this->attackWasteMana = attackWasteMana;

    attackDamageText.loadFromRenderedText(std::to_string(attackDamage), SDL_Color{255, 0, 0, SDL_ALPHA_OPAQUE}, gFontGame, gRenderer);
    attackNameText.loadFromRenderedText(attackName, SDL_Color{0, 0, 0, SDL_ALPHA_OPAQUE}, gFontGame, gRenderer);
    attackWasteManaText.loadFromRenderedText(std::to_string(attackWasteMana), SDL_Color{0, 66, 225, SDL_ALPHA_OPAQUE}, gFontGame, gRenderer);
    attackDescriptionText.loadFromRenderedText(attackDescription, SDL_Color{0, 0, 0, SDL_ALPHA_OPAQUE}, gFontGame, gRenderer);
}

Attack::~Attack()
{
    attackDamage = 0;
    attackName = "";
    attackDescription = "";
    attackWasteMana = 0;

    attackDamageText.free();
    attackNameText.free();
    attackDescriptionText.free();
    attackWasteManaText.free();
}
