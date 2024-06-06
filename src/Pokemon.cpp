#include "Pokemon.hpp"

Pokemon::Pokemon()
{
    name = "";
    fullHp = 0;
    hp = 0;
    fullMana = 0;
    mana = 0;
}

Pokemon::~Pokemon()
{
    name = "";
    fullHp = 0;
    hp = 0;
    fullMana = 0;
    mana = 0;
    texture.free();
    nameText.free();
    hpText.free();
    manaText.free();
}

void Pokemon::init(std::string name, int hp, int mana, PokemonTypes type, SDL_Renderer *gRenderer, TTF_Font *font)
{
    this->name = name;
    this->fullHp = hp;
    this->fullMana = mana;
    this->hp = hp;
    this->mana = mana;
    this->type = type;

    nameText.loadFromRenderedText(name, SDL_Color{0, 0, 0, SDL_ALPHA_OPAQUE}, font, gRenderer);
    hpText.loadFromRenderedText(std::to_string(hp), SDL_Color{201, 15, 15, SDL_ALPHA_OPAQUE}, font, gRenderer);
    manaText.loadFromRenderedText(std::to_string(mana), SDL_Color{40, 111, 254, SDL_ALPHA_OPAQUE}, font, gRenderer);

    if (name == "Blastoise")
    {
        texture.loadFromFile("/home/stas/SDL/PokemonUpdate/assets/Images/blastoise_main.png", gRenderer);
        textureBattle.loadFromFile("/home/stas/SDL/PokemonUpdate/assets/Images/blastoise_battle.png", gRenderer);
    }
    else if (name == "Cacturne")
    {

        texture.loadFromFile("/home/stas/SDL/PokemonUpdate/assets/Images/cacturne_main.png", gRenderer);
        textureBattle.loadFromFile("/home/stas/SDL/PokemonUpdate/assets/Images/cacturne_battle.png", gRenderer);
    }
    else if (name == "Camerupt")
    {
        texture.loadFromFile("/home/stas/SDL/PokemonUpdate/assets/Images/camerupt_main.png", gRenderer);
        textureBattle.loadFromFile("/home/stas/SDL/PokemonUpdate/assets/Images/camerupt_battle.png", gRenderer);
    }
    else if (name == "Charizard")
    {
        texture.loadFromFile("/home/stas/SDL/PokemonUpdate/assets/Images/charizard_main.png", gRenderer);
        textureBattle.loadFromFile("/home/stas/SDL/PokemonUpdate/assets/Images/charizard_battle.png", gRenderer);
    }
    else if (name == "Golduck")
    {
        texture.loadFromFile("/home/stas/SDL/PokemonUpdate/assets/Images/golduck_main.png", gRenderer);
        textureBattle.loadFromFile("/home/stas/SDL/PokemonUpdate/assets/Images/golduck_battle.png", gRenderer);
    }
    else if (name == "Venusaur")
    {
        texture.loadFromFile("/home/stas/SDL/PokemonUpdate/assets/Images/venusaur_main.png", gRenderer);
        textureBattle.loadFromFile("/home/stas/SDL/PokemonUpdate/assets/Images/venusaur_battle.png", gRenderer);
    }
}