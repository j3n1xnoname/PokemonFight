#include "Game.hpp"

bool Game::fillSounds()
{
    bool success = true;

    chunks[CHUNK_ENTER] = Mix_LoadWAV("assets/Sounds/enterButton.mp3");
    if (chunks[CHUNK_ENTER] == NULL)
    {
        success = false;
        printf("Failed to load sound effect(enterButton)! SDL_mixer Error: %s\n", Mix_GetError());
    }

    chunks[CHUNK_PRESS_ARROW] = Mix_LoadWAV("assets/Sounds/pressArrow.mp3");
    if (chunks[CHUNK_PRESS_ARROW] == NULL)
    {
        success = false;
        printf("Failed to load sound effect(pressArrow)! SDL_mixer Error: %s\n", Mix_GetError());
    }

    chunks[CHUNK_PRESS_ENTER_MENU] = Mix_LoadWAV("assets/Sounds/pressEnterMenu.mp3");
    if (chunks[CHUNK_PRESS_ENTER_MENU] == NULL)
    {
        success = false;
        printf("Failed to load sound effect(pressEnterMenu)! SDL_mixer Error: %s\n", Mix_GetError());
    }

    chunks[CHUNK_HIT] = Mix_LoadWAV("assets/Sounds/hit.wav");
    if (chunks[CHUNK_HIT] == NULL)
    {
        success = false;
        printf("Failed to load sound effect(hit)! SDL_mixer Error: %s\n", Mix_GetError());
    }

    musics[MUSIC_BATTLE] = Mix_LoadMUS("assets/Sounds/battleMusic.mp3");
    if (musics[MUSIC_BATTLE] == NULL)
    {
        success = false;
        printf("Faield to load music(battleMusic)! SDL_Mixer Erorr: %s\n", Mix_GetError());
    }

    musics[MUSIC_END] = Mix_LoadMUS("assets/Sounds/endMusic.wav");
    if (musics[MUSIC_END] == NULL)
    {
        success = false;
        printf("Faield to load music(endMusic)! SDL_Mixer Erorr: %s\n", Mix_GetError());
    }

    musics[MUSIC_WINNER] = Mix_LoadMUS("assets/Sounds/winnerMusic.mp3");
    if (musics[MUSIC_WINNER] == NULL)
    {
        success = false;
        printf("Faield to load music(winnerMusic)! SDL_Mixer Erorr: %s\n", Mix_GetError());
    }

    return success;
}

bool Game::fillIcons()
{
    bool success = true;
    if (!icons[ICON_ARROW_DOWN].loadFromFile("assets/Images/arrow_down1.png", gRenderer))
    {
        success = false;
        printf("Failed to load arrow_right.png!\n");
    }

    if (!icons[ICON_HEART_BIG].loadFromFile("assets/Images/heart_big.png", gRenderer))
    {
        success = false;
        printf("Failed to load heart_big.png!\n");
    }

    if (!icons[ICON_BOTTLE_BIG].loadFromFile("assets/Images/bottle_big.png", gRenderer))
    {
        success = false;
        printf("Failed to load bottle_big.png!\n");
    }

    if (!icons[ICON_BALL_FIRE].loadFromFile("assets/Images/ball_fire.png", gRenderer))
    {
        success = false;
        printf("Failed to load ball_fire.png!\n");
    }

    if (!icons[ICON_BALL_WATER].loadFromFile("assets/Images/ball_water.png", gRenderer))
    {
        success = false;
        printf("Failed to load ball_water.png!\n");
    }

    if (!icons[ICON_BALL_GRASS].loadFromFile("assets/Images/ball_grass.png", gRenderer))
    {
        success = false;
        printf("Failed to load ball_grass.png!\n");
    }

    if (!icons[ICON_HEALTHBAR].loadFromFile("assets/Images/healthbar.png", gRenderer))
    {
        success = false;
        printf("Failed to load healthbar.png!\n");
    }

    if (!icons[ICON_HEART_SMALL].loadFromFile("assets/Images/heart_mini.png", gRenderer))
    {
        success = false;
        printf("Failed to load heart_mini.png!\n");
    }

    if (!icons[ICON_MANA_BOTTLE].loadFromFile("assets/Images/manaBottle.png", gRenderer))
    {
        success = false;
        printf("Failed to load manaBottle.png!\n");
    }

    if (!icons[ICON_ARROW_LEFT].loadFromFile("assets/Images/arrow_left.png", gRenderer))
    {
        success = false;
        printf("Failed to load arrow_left.png!\n");
    }

    if (!icons[ICON_ARROW_RIGHT].loadFromFile("assets/Images/arrow_right.png", gRenderer))
    {
        success = false;
        printf("Failed to load arrow_right.png!\n");
    }

    if (!icons[ICON_ATTACK].loadFromFile("assets/Images/attack.png", gRenderer))
    {
        success = false;
        printf("Failed to load attack.png!\n");
    }

    if (!icons[ICON_CROSS].loadFromFile("assets/Images/cross.png", gRenderer))
    {
        success = false;
        printf("Failed to load cross.png!\n");
    }

    if (!icons[ICON_CROWN].loadFromFile("assets/Images/crown.png", gRenderer))
    {
        success = false;
        printf("Failed to load cross.png!\n");
    }
    return success;
}

bool Game::fillTexts()
{
    bool success = true;
    if (!texts[TEXT_PRESS_ENTER].loadFromRenderedText("Press enter to start game...", SDL_Color{232, 253, 96, SDL_ALPHA_OPAQUE}, fonts[FONT_48], gRenderer))
    {
        success = false;
        printf("Failed to load TEXT_PRESS_ENTER\n");
    }

    if (!texts[TEXT_WATER].loadFromRenderedText("Water", SDL_Color{42, 106, 210, SDL_ALPHA_OPAQUE}, fonts[FONT_BATTLE], gRenderer))
    {
        success = false;
        printf("Failed to load TEXT_WATER\n");
    }
    if (!texts[TEXT_GRASS].loadFromRenderedText("Grass", SDL_Color{11, 171, 114, SDL_ALPHA_OPAQUE}, fonts[FONT_BATTLE], gRenderer))
    {
        success = false;
        printf("Failed to load TEXT_GRASS\n");
    }
    if (!texts[TEXT_FIRE].loadFromRenderedText("Fire", SDL_Color{189, 17, 17, SDL_ALPHA_OPAQUE}, fonts[FONT_BATTLE], gRenderer))
    {
        success = false;
        printf("Failed to load TEXT_FIRE\n");
    }
    if (!texts[TEXT_PLAYER_ONE].loadFromRenderedText("P1", SDL_Color{64, 96, 255, SDL_ALPHA_OPAQUE}, fonts[FONT_BATTLE], gRenderer))
    {
        success = false;
        printf("Failed to load P1!\n");
    }
    if (!texts[TEXT_PLAYER_TWO].loadFromRenderedText("P2", SDL_Color{255, 21, 21, SDL_ALPHA_OPAQUE}, fonts[FONT_BATTLE], gRenderer))
    {
        success = false;
        printf("Failed to load P2!\n");
    }

    if (!texts[TEXT_AUTHOR_NICKNAME].loadFromRenderedText("J3N1X", SDL_Color{0, 0, 0, SDL_ALPHA_OPAQUE}, fonts[FONT_TUSJ], gRenderer))
    {
        success = false;
        printf("Failed to load TEXT_AUTHOR_NICKNAME!\n");
    }

    if (!texts[TEXT_GAME_WAS_MADE].loadFromRenderedText("The game was made by", SDL_Color{0, 0, 0, SDL_ALPHA_OPAQUE}, fonts[FONT_SEASRN], gRenderer))
    {
        success = false;
        printf("Failed to load TEXT_GAME_WAS_MADE!\n");
    }

    if (!texts[TEXT_POKEMON_FIGHT].loadFromRenderedText("Pokemon Fight", SDL_Color{0, 0, 0, SDL_ALPHA_OPAQUE}, fonts[FONT_SEASRN], gRenderer))
    {
        success = false;
        printf("Failed to load TEXT_POKEMON_FIGHT!\n");
    }

    return success;
}

void Game::fillPokemons()
{
    std::ifstream in("assets/pokemons.txt");

    std::string s;

    if (in.is_open())
    {

        for (int i = 0; i < COUNT_POKEMONS; i++)
        {
            std::string name, type, attackName, attackDescription;
            int hp, mana, attackDamage, attackWasteMana;
            std::getline(in, s);
            name = s;
            std::getline(in, s);
            type = s;
            std::getline(in, s);
            hp = std::stoi(s);
            std::getline(in, s);
            mana = std::stoi(s);
            int tempType;
            if (type == "Water")
                tempType = TYPE_WATER;
            else if (type == "Grass")
                tempType = TYPE_GRASS;
            else if (type == "Fire")
                tempType = TYPE_FIRE;
            pokemons[i].init(name, hp, mana, tempType, gRenderer, fonts[FONT_BATTLE]);
            for (int j = 0; j < 4; j++)
            {
                std::getline(in, s);
                attackName = s;
                std::getline(in, s);
                attackDescription = s;
                std::getline(in, s);
                attackDamage = std::stoi(s);
                std::getline(in, s);
                attackWasteMana = std::stoi(s);
                pokemons[i].attacks[j].init(attackDamage, attackName, attackDescription, attackWasteMana, gRenderer, fonts[FONT_BATTLE]);
            }
            pokemons[i].attacks[4].init(0, "Skip Move", "Just skip a move", 0, gRenderer, fonts[FONT_BATTLE]);
            std::getline(in, s);
        }
    }
    else
    {
        printf("Error opening file!\n");
    }
    in.close();
}

bool Game::fillFonts()
{
    bool success = true;
    TTF_Font *gFontGame = NULL;
    gFontGame = TTF_OpenFont("assets/Fonts/ComicoroRu_0.ttf", 64);
    if (gFontGame == NULL)
    {
        printf("TTF failed to create font. TTF_Error: %s\n", TTF_GetError());
        success = false;
    }

    TTF_Font *gFont48 = NULL;
    gFont48 = TTF_OpenFont("assets/Fonts/ChunkFive-Regular.otf", 48);
    if (gFont48 == NULL)
    {
        printf("TTF failed to create font. TTF_Error: %s\n", TTF_GetError());
        success = false;
    }
    TTF_Font *gFontGame48 = NULL;
    gFontGame48 = TTF_OpenFont("assets/Fonts/ComicoroRu_0.ttf", 48);
    if (gFontGame48 == NULL)
    {
        printf("TTF failed to create font. TTF_Error: %s\n", TTF_GetError());
        success = false;
    }
    TTF_Font *gFontGame64 = NULL;
    gFontGame64 = TTF_OpenFont("assets/Fonts/ComicoroRu_0.ttf", 64);
    if (gFontGame64 == NULL)
    {
        printf("TTF failed to create font. TTF_Error: %s\n", TTF_GetError());
        success = false;
    }

    TTF_Font *gFontEnd1 = NULL;
    gFontEnd1 = TTF_OpenFont("assets/Fonts/SEASRN.ttf", 64);
    if (gFontEnd1 == NULL)
    {
        printf("TTF failed to create font. TTF_Error: %s\n", TTF_GetError());
        success = false;
    }

    TTF_Font *gFontEnd2 = NULL;
    gFontEnd2 = TTF_OpenFont("assets/Fonts/FFF_Tusj.ttf", 64);
    if (gFontEnd2 == NULL)
    {
        printf("TTF failed to create font. TTF_Error: %s\n", TTF_GetError());
        success = false;
    }

    fonts[FONT_BATTLE] = gFontGame;
    fonts[FONT_48] = gFont48;
    fonts[FONT_BATTLE_48] = gFontGame48;
    fonts[FONT_BATTLE_64] = gFontGame64;
    fonts[FONT_SEASRN] = gFontEnd1;
    fonts[FONT_TUSJ] = gFontEnd2;
    return success;
}

bool Game::loadAllMedia()
{
    bool success = true;
    if (!fillFonts())
    {
        success = false;
    }

    if (!fillIcons())
    {
        success = false;
    }

    if (!fillSounds())
    {
        success = false;
    }
    return success;
}