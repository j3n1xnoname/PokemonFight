#include "Game.hpp"

void Game::display()
{
    switch (globalState)
    {
    case STATE_START:
    {
        texts[TEXT_PRESS_ENTER].render(SCREEN_WIDTH / 2 - texts[TEXT_PRESS_ENTER].mWidth / 2,
                                       SCREEN_HEIGHT / 2 - texts[TEXT_PRESS_ENTER].mHeight / 2, gRenderer);
        break;
    }
    case STATE_CHOOSE_POKEMONS:
    {
        // width(W) it's x pos;  height(H) it's y pos;
        int blastW = SCREEN_WIDTH / 60, blastH = SCREEN_HEIGHT / 2 - pokemons[POKEMON_BLASTOISE].texture.mHeight;
        int cactW = SCREEN_WIDTH * 10 / 60, cactH = SCREEN_HEIGHT / 2 - pokemons[POKEMON_CACTURNE].texture.mHeight;
        int camerW = SCREEN_WIDTH * 20 / 60, camerH = SCREEN_HEIGHT / 2 - pokemons[POKEMON_CAMERUPT].texture.mHeight;
        int charW = SCREEN_WIDTH * 30 / 60, charH = SCREEN_HEIGHT / 2 - pokemons[POKEMON_CHARIZARD].texture.mHeight;
        int goldW = SCREEN_WIDTH * 40 / 60, goldH = SCREEN_HEIGHT / 2 - pokemons[POKEMON_GOLDUCK].texture.mHeight;
        int venusW = SCREEN_WIDTH * 50 / 60, venusH = SCREEN_HEIGHT / 2 - pokemons[POKEMON_VENUSAUR].texture.mHeight;

        std::vector<std::pair<int, int>> position;
        position.push_back({blastW, blastH});
        position.push_back({cactW, cactH});
        position.push_back({camerW, camerH});
        position.push_back({charW, charH});
        position.push_back({goldW, goldH});
        position.push_back({venusW, venusH});

        for (int i = 0; i < COUNT_POKEMONS; i++)
        {
            pokemons[i].texture.render(position[i].first, position[i].second, gRenderer);
            icons[ICON_HEART_BIG].render(position[i].first, position[i].second * 5 / 3, gRenderer);
            pokemons[i].nameText.render(position[i].first + pokemons[i].nameText.mWidth * 5 / 18,
                                        position[i].second - pokemons[i].nameText.mHeight, gRenderer);
            icons[ICON_BOTTLE_BIG].render(position[i].first, position[i].second * 2, gRenderer);

            pokemons[i].hpText.render(position[i].first + icons[ICON_HEART_BIG].mWidth, position[i].second * 5 / 3 + icons[ICON_HEART_BIG].mWidth / 3, gRenderer);

            pokemons[i].manaText.render(position[i].first + icons[ICON_BOTTLE_BIG].mWidth, position[i].second * 5 / 3 + icons[ICON_BOTTLE_BIG].mWidth * 4 / 3, gRenderer);
        }

        int arrow_width, arrow_height;
        arrow_width = blastW * 3 + (int)curIndexPokemon * 10 * SCREEN_WIDTH / 60;
        arrow_height = (SCREEN_HEIGHT / 2 - pokemons[(int)curIndexPokemon].texture.mHeight) / 2;
        if (curIndexPokemon == POKEMON_BLASTOISE)
        {
            arrow_width = blastW * 4;
            arrow_height = blastH / 2;
        }

        if (currentPlayerNumber == 1)
        {
            texts[TEXT_PLAYER_ONE].render(arrow_width + texts[TEXT_PLAYER_ONE].mWidth / 2, arrow_height - texts[TEXT_PLAYER_ONE].mHeight, gRenderer);
        }
        else if (currentPlayerNumber == 2)
        {
            texts[TEXT_PLAYER_TWO].render(arrow_width + texts[TEXT_PLAYER_TWO].mWidth / 2, arrow_height - texts[TEXT_PLAYER_TWO].mHeight, gRenderer);
        }

        icons[ICON_ARROW_DOWN].render(arrow_width, arrow_height, gRenderer);

        LTexture *currentTypeIcon;
        LTexture *currentTypeText;
        for (int i = 0; i < COUNT_POKEMONS; i++)
        {
            if (pokemons[i].type == TYPE_FIRE)
            {
                currentTypeIcon = &icons[ICON_BALL_FIRE];
                currentTypeText = &texts[TEXT_FIRE];
            }
            else if (pokemons[i].type == TYPE_GRASS)
            {
                currentTypeIcon = &icons[ICON_BALL_GRASS];
                currentTypeText = &texts[TEXT_GRASS];
            }
            else if (pokemons[i].type == TYPE_WATER)
            {
                currentTypeIcon = &icons[ICON_BALL_WATER];
                currentTypeText = &texts[TEXT_WATER];
            }
            currentTypeIcon->render(position[i].first + icons[ICON_BOTTLE_BIG].mWidth, position[i].second * 2 + icons[ICON_BOTTLE_BIG].mHeight, gRenderer);
            if (pokemons[i].type == TYPE_FIRE)
            {
                currentTypeText->render(position[i].first + currentTypeText->mWidth * 13 / 10, position[i].second * 2 + 2 * icons[ICON_BOTTLE_BIG].mHeight, gRenderer);
            }
            else
            {
                currentTypeText->render(position[i].first + currentTypeText->mWidth * 3 / 4, position[i].second * 2 + 2 * icons[ICON_BOTTLE_BIG].mHeight, gRenderer);
            }
        }
        break;
    }
    case STATE_BATTLE:
    {
        SDL_Rect tempRect;
        tempRect.w = 200;
        tempRect.h = 200;
        tempRect.x = 0;
        tempRect.y = 0;

        SDL_Point tempPoint;
        tempPoint.x = 0;
        tempPoint.y = 0;

        players[PLAYER_1].playerPokemons[INDEX_POKEMON_ONE].textureBattle.render(SCREEN_WIDTH / 30, SCREEN_HEIGHT / 40, gRenderer, &tempRect, 0, &tempPoint, SDL_FLIP_HORIZONTAL);
        displayPokemonType(players[PLAYER_1].playerPokemons[INDEX_POKEMON_ONE], SCREEN_WIDTH / 30 - icons[ICON_BALL_FIRE].mWidth / 2, SCREEN_HEIGHT / 40);
        players[PLAYER_1].playerPokemons[INDEX_POKEMON_TWO].textureBattle.render(SCREEN_WIDTH / 30, SCREEN_HEIGHT * 15 / 40, gRenderer, &tempRect, 0, &tempPoint, SDL_FLIP_HORIZONTAL);
        displayPokemonType(players[PLAYER_1].playerPokemons[INDEX_POKEMON_TWO], SCREEN_WIDTH / 30 - icons[ICON_BALL_FIRE].mWidth / 2, SCREEN_HEIGHT * 15 / 40);
        players[PLAYER_1].playerPokemons[INDEX_POKEMON_THREE].textureBattle.render(SCREEN_WIDTH / 30, SCREEN_HEIGHT * 27 / 40, gRenderer, &tempRect, 0, &tempPoint, SDL_FLIP_HORIZONTAL);
        displayPokemonType(players[PLAYER_1].playerPokemons[INDEX_POKEMON_THREE], SCREEN_WIDTH / 30 - icons[ICON_BALL_FIRE].mWidth / 2, SCREEN_HEIGHT * 27 / 40);

        players[PLAYER_2].playerPokemons[INDEX_POKEMON_ONE].textureBattle.render(SCREEN_WIDTH * 25 / 30, SCREEN_HEIGHT / 40, gRenderer, &tempRect);
        displayPokemonType(players[PLAYER_2].playerPokemons[INDEX_POKEMON_ONE], SCREEN_WIDTH * 25 / 30 - icons[ICON_BALL_FIRE].mWidth, SCREEN_HEIGHT / 40);
        players[PLAYER_2].playerPokemons[INDEX_POKEMON_TWO].textureBattle.render(SCREEN_WIDTH * 25 / 30, SCREEN_HEIGHT * 15 / 40, gRenderer, &tempRect);
        displayPokemonType(players[PLAYER_2].playerPokemons[INDEX_POKEMON_TWO], SCREEN_WIDTH * 25 / 30 - icons[ICON_BALL_FIRE].mWidth, SCREEN_HEIGHT * 15 / 40);
        players[PLAYER_2].playerPokemons[INDEX_POKEMON_THREE].textureBattle.render(SCREEN_WIDTH * 25 / 30, SCREEN_HEIGHT * 27 / 40, gRenderer, &tempRect);
        displayPokemonType(players[PLAYER_2].playerPokemons[INDEX_POKEMON_THREE], SCREEN_WIDTH * 25 / 30 - icons[ICON_BALL_FIRE].mWidth, SCREEN_HEIGHT * 27 / 40);

        players[PLAYER_1].playerPokemons[INDEX_POKEMON_ONE].nameText.render(SCREEN_WIDTH / 30, SCREEN_HEIGHT / 40 - players[PLAYER_1].playerPokemons[INDEX_POKEMON_ONE].nameText.mHeight * 23 / 32, gRenderer);
        players[PLAYER_1].playerPokemons[INDEX_POKEMON_TWO].nameText.render(SCREEN_WIDTH / 30, SCREEN_HEIGHT * 15 / 40 - players[PLAYER_1].playerPokemons[INDEX_POKEMON_TWO].nameText.mHeight * 23 / 32, gRenderer);
        players[PLAYER_1].playerPokemons[INDEX_POKEMON_THREE].nameText.render(SCREEN_WIDTH / 30, SCREEN_HEIGHT * 27 / 40 - players[PLAYER_1].playerPokemons[INDEX_POKEMON_THREE].nameText.mHeight * 23 / 32, gRenderer);

        players[PLAYER_2].playerPokemons[INDEX_POKEMON_ONE].nameText.render(SCREEN_WIDTH * 25 / 30, SCREEN_HEIGHT / 40 - players[PLAYER_2].playerPokemons[INDEX_POKEMON_ONE].nameText.mHeight * 23 / 32, gRenderer);
        players[PLAYER_2].playerPokemons[INDEX_POKEMON_TWO].nameText.render(SCREEN_WIDTH * 25 / 30, SCREEN_HEIGHT * 15 / 40 - players[PLAYER_2].playerPokemons[INDEX_POKEMON_TWO].nameText.mHeight * 23 / 32, gRenderer);
        players[PLAYER_2].playerPokemons[INDEX_POKEMON_THREE].nameText.render(SCREEN_WIDTH * 25 / 30, SCREEN_HEIGHT * 27 / 40 - players[PLAYER_2].playerPokemons[INDEX_POKEMON_THREE].nameText.mHeight * 23 / 32, gRenderer);

        displayArrow();
        crossOutDeadPokemons();

        if (battleState == STATE_BATTLE_CHOOSE_ATTACK)
        {
            displayAttacks(currentPlayer);
            if (currentPlayerNumber == 1)
            {
                addTextToScreenRightAway("Choose Attack!", SDL_Color{64, 96, 255, SDL_ALPHA_OPAQUE}, fonts[FONT_BATTLE_64], gRenderer,
                                         SCREEN_WIDTH * 27 / 64, 0);
            }
            else
            {
                addTextToScreenRightAway("Choose Attack!", SDL_Color{255, 21, 21, SDL_ALPHA_OPAQUE}, fonts[FONT_BATTLE_64], gRenderer,
                                         SCREEN_WIDTH * 27 / 64, 0);
            }
        }

        if (battleState == STATE_BATTLE_CHOOSE_POKEMON)
        {

            if (currentPlayerNumber == 1)
            {
                addTextToScreenRightAway("Choose pokemon", SDL_Color{64, 96, 255, SDL_ALPHA_OPAQUE}, fonts[FONT_BATTLE_64], gRenderer,
                                         SCREEN_WIDTH * 27 / 64, 0);
            }
            else
            {
                addTextToScreenRightAway("Choose pokemon", SDL_Color{255, 21, 21, SDL_ALPHA_OPAQUE}, fonts[FONT_BATTLE_64], gRenderer,
                                         SCREEN_WIDTH * 27 / 64, 0);
            }
        }

        if (battleState == STATE_BATTLE_CHOOSE_ENEMY_POKEMON)
        {

            if (currentPlayerNumber == 1)
            {
                addTextToScreenRightAway("Choose enemy pokemon", SDL_Color{64, 96, 255, SDL_ALPHA_OPAQUE}, fonts[FONT_BATTLE_64], gRenderer,
                                         SCREEN_WIDTH * 25 / 64, 0);
            }
            else
            {
                addTextToScreenRightAway("Choose enemy pokemon", SDL_Color{255, 21, 21, SDL_ALPHA_OPAQUE}, fonts[FONT_BATTLE_64], gRenderer,
                                         SCREEN_WIDTH * 25 / 64, 0);
            }
            if (curIndexAttack != ATTACK_FIFTH && anotherPlayer->playerPokemons[(int)*anotherPlayerPokemonIndex].isAlive)
            {
                std::string tempString = "";
                tempString += std::to_string(currentPlayer->playerPokemons[(int)*playerPokemonIndex].attacks[(int)curIndexAttack].attackDamage);
                tempString += " * ";
                float tempNumber = attackCoefficents[currentPlayer->playerPokemons[(int)*playerPokemonIndex].type][anotherPlayer->playerPokemons[(int)*anotherPlayerPokemonIndex].type];
                char formattedNumber[10];
                sprintf(formattedNumber, "%.1f", tempNumber);
                tempString += formattedNumber;
                // tempString += std::to_string(attackCoefficents[currentPlayer->playerPokemons[*playerPokemonIndex].type][anotherPlayer->playerPokemons[*anotherPlayerPokemonIndex].type]);
                tempString += " = ";
                tempString += std::to_string((int)(currentPlayer->playerPokemons[(int)*playerPokemonIndex].attacks[(int)curIndexAttack].attackDamage * attackCoefficents[currentPlayer->playerPokemons[(int)*playerPokemonIndex].type][anotherPlayer->playerPokemons[(int)*anotherPlayerPokemonIndex].type]));
                SDL_Color color;
                if (currentPlayerNumber == 1)
                {
                    color = {64, 96, 255, SDL_ALPHA_OPAQUE};
                }
                else
                {
                    color = SDL_Color{255, 21, 21, SDL_ALPHA_OPAQUE};
                }

                addTextToScreenRightAway(tempString, color, fonts[FONT_BATTLE_64], gRenderer, SCREEN_WIDTH * 13 / 32, SCREEN_HEIGHT * 7 / 16);
                addTextToScreenRightAway("Damage:", SDL_Color{0, 0, 0, SDL_ALPHA_OPAQUE}, fonts[FONT_BATTLE_64], gRenderer, SCREEN_WIDTH * 10 / 32, SCREEN_HEIGHT * 7 / 16);
            }
        }

        texts[TEXT_PLAYER_ONE].render(0, -14, gRenderer);
        texts[TEXT_PLAYER_TWO].render(SCREEN_WIDTH - texts[TEXT_PLAYER_TWO].mWidth, -14, gRenderer);

        icons[ICON_HEALTHBAR].render(SCREEN_WIDTH / 30, SCREEN_HEIGHT / 40 + 201, gRenderer);
        icons[ICON_HEALTHBAR].render(SCREEN_WIDTH / 30, SCREEN_HEIGHT / 40 + 203 + icons[ICON_HEALTHBAR].mHeight, gRenderer);

        icons[ICON_HEALTHBAR].render(SCREEN_WIDTH / 30, SCREEN_HEIGHT * 15 / 40 + 201, gRenderer);
        icons[ICON_HEALTHBAR].render(SCREEN_WIDTH / 30, SCREEN_HEIGHT * 15 / 40 + 203 + icons[ICON_HEALTHBAR].mHeight, gRenderer);

        icons[ICON_HEALTHBAR].render(SCREEN_WIDTH / 30, SCREEN_HEIGHT * 27 / 40 + 201, gRenderer);
        icons[ICON_HEALTHBAR].render(SCREEN_WIDTH / 30, SCREEN_HEIGHT * 27 / 40 + 203 + icons[ICON_HEALTHBAR].mHeight, gRenderer);

        icons[ICON_HEALTHBAR].render(SCREEN_WIDTH * 25 / 30, SCREEN_HEIGHT / 40 + 201, gRenderer);
        icons[ICON_HEALTHBAR].render(SCREEN_WIDTH * 25 / 30, SCREEN_HEIGHT / 40 + 203 + icons[ICON_HEALTHBAR].mHeight, gRenderer);

        icons[ICON_HEALTHBAR].render(SCREEN_WIDTH * 25 / 30, SCREEN_HEIGHT * 15 / 40 + 201, gRenderer);
        icons[ICON_HEALTHBAR].render(SCREEN_WIDTH * 25 / 30, SCREEN_HEIGHT * 15 / 40 + 203 + icons[ICON_HEALTHBAR].mHeight, gRenderer);

        icons[ICON_HEALTHBAR].render(SCREEN_WIDTH * 25 / 30, SCREEN_HEIGHT * 27 / 40 + 201, gRenderer);
        icons[ICON_HEALTHBAR].render(SCREEN_WIDTH * 25 / 30, SCREEN_HEIGHT * 27 / 40 + 203 + icons[ICON_HEALTHBAR].mHeight, gRenderer);

        addTextToScreenRightAway(std::to_string(players[PLAYER_1].playerPokemons[INDEX_POKEMON_ONE].hp), SDL_Color{201, 15, 15, SDL_ALPHA_OPAQUE}, fonts[FONT_BATTLE_48], gRenderer,
                                 SCREEN_WIDTH / 30 + icons[ICON_HEALTHBAR].mWidth + 3, SCREEN_HEIGHT / 40 + 189);
        addTextToScreenRightAway(std::to_string(players[PLAYER_1].playerPokemons[INDEX_POKEMON_ONE].mana), SDL_Color{0, 66, 225, SDL_ALPHA_OPAQUE}, fonts[FONT_BATTLE_48], gRenderer,
                                 SCREEN_WIDTH / 30 + icons[ICON_HEALTHBAR].mWidth + 3, SCREEN_HEIGHT / 40 + 213);

        addTextToScreenRightAway(std::to_string(players[PLAYER_1].playerPokemons[INDEX_POKEMON_TWO].hp), SDL_Color{201, 15, 15, SDL_ALPHA_OPAQUE}, fonts[FONT_BATTLE_48], gRenderer,
                                 SCREEN_WIDTH / 30 + icons[ICON_HEALTHBAR].mWidth + 3, SCREEN_HEIGHT * 15 / 40 + 189);
        addTextToScreenRightAway(std::to_string(players[PLAYER_1].playerPokemons[INDEX_POKEMON_TWO].mana), SDL_Color{0, 66, 225, SDL_ALPHA_OPAQUE}, fonts[FONT_BATTLE_48], gRenderer,
                                 SCREEN_WIDTH / 30 + icons[ICON_HEALTHBAR].mWidth + 3, SCREEN_HEIGHT * 15 / 40 + 213);

        addTextToScreenRightAway(std::to_string(players[PLAYER_1].playerPokemons[INDEX_POKEMON_THREE].hp), SDL_Color{201, 15, 15, SDL_ALPHA_OPAQUE}, fonts[FONT_BATTLE_48], gRenderer,
                                 SCREEN_WIDTH / 30 + icons[ICON_HEALTHBAR].mWidth + 3, SCREEN_HEIGHT * 27 / 40 + 189);
        addTextToScreenRightAway(std::to_string(players[PLAYER_1].playerPokemons[INDEX_POKEMON_THREE].mana), SDL_Color{0, 66, 225, SDL_ALPHA_OPAQUE}, fonts[FONT_BATTLE_48], gRenderer,
                                 SCREEN_WIDTH / 30 + icons[ICON_HEALTHBAR].mWidth + 3, SCREEN_HEIGHT * 27 / 40 + 213);

        addTextToScreenRightAway(std::to_string(players[PLAYER_2].playerPokemons[INDEX_POKEMON_ONE].hp), SDL_Color{201, 15, 15, SDL_ALPHA_OPAQUE}, fonts[FONT_BATTLE_48], gRenderer,
                                 SCREEN_WIDTH * 25 / 30 + icons[ICON_HEALTHBAR].mWidth + 3, SCREEN_HEIGHT / 40 + 189);
        addTextToScreenRightAway(std::to_string(players[PLAYER_2].playerPokemons[INDEX_POKEMON_ONE].mana), SDL_Color{0, 66, 225, SDL_ALPHA_OPAQUE}, fonts[FONT_BATTLE_48], gRenderer,
                                 SCREEN_WIDTH * 25 / 30 + icons[ICON_HEALTHBAR].mWidth + 3, SCREEN_HEIGHT / 40 + 213);

        addTextToScreenRightAway(std::to_string(players[PLAYER_2].playerPokemons[INDEX_POKEMON_TWO].hp), SDL_Color{201, 15, 15, SDL_ALPHA_OPAQUE}, fonts[FONT_BATTLE_48], gRenderer,
                                 SCREEN_WIDTH * 25 / 30 + icons[ICON_HEALTHBAR].mWidth + 3, SCREEN_HEIGHT * 15 / 40 + 189);
        addTextToScreenRightAway(std::to_string(players[PLAYER_2].playerPokemons[INDEX_POKEMON_TWO].mana), SDL_Color{0, 66, 225, SDL_ALPHA_OPAQUE}, fonts[FONT_BATTLE_48], gRenderer,
                                 SCREEN_WIDTH * 25 / 30 + icons[ICON_HEALTHBAR].mWidth + 3, SCREEN_HEIGHT * 15 / 40 + 213);

        addTextToScreenRightAway(std::to_string(players[PLAYER_2].playerPokemons[INDEX_POKEMON_THREE].hp), SDL_Color{201, 15, 15, SDL_ALPHA_OPAQUE}, fonts[FONT_BATTLE_48], gRenderer,
                                 SCREEN_WIDTH * 25 / 30 + icons[ICON_HEALTHBAR].mWidth + 3, SCREEN_HEIGHT * 27 / 40 + 189);
        addTextToScreenRightAway(std::to_string(players[PLAYER_2].playerPokemons[INDEX_POKEMON_THREE].mana), SDL_Color{0, 66, 225, SDL_ALPHA_OPAQUE}, fonts[FONT_BATTLE_48], gRenderer,
                                 SCREEN_WIDTH * 25 / 30 + icons[ICON_HEALTHBAR].mWidth + 3, SCREEN_HEIGHT * 27 / 40 + 213);

        icons[ICON_HEART_SMALL].render(SCREEN_WIDTH / 30 - icons[ICON_HEART_SMALL].mWidth * 7 / 8, SCREEN_HEIGHT / 40 + 201 - icons[ICON_HEART_SMALL].mHeight / 2, gRenderer);
        icons[ICON_HEART_SMALL].render(SCREEN_WIDTH / 30 - icons[ICON_HEART_SMALL].mWidth * 7 / 8, SCREEN_HEIGHT * 15 / 40 + 201 - icons[ICON_HEART_SMALL].mHeight / 2, gRenderer);
        icons[ICON_HEART_SMALL].render(SCREEN_WIDTH / 30 - icons[ICON_HEART_SMALL].mWidth * 7 / 8, SCREEN_HEIGHT * 27 / 40 + 201 - icons[ICON_HEART_SMALL].mHeight / 2, gRenderer);
        icons[ICON_HEART_SMALL].render(SCREEN_WIDTH * 25 / 30 - icons[ICON_HEART_SMALL].mWidth * 7 / 8, SCREEN_HEIGHT / 40 + 201 - icons[ICON_HEART_SMALL].mHeight / 2, gRenderer);
        icons[ICON_HEART_SMALL].render(SCREEN_WIDTH * 25 / 30 - icons[ICON_HEART_SMALL].mWidth * 7 / 8, SCREEN_HEIGHT * 15 / 40 + 201 - icons[ICON_HEART_SMALL].mHeight / 2, gRenderer);
        icons[ICON_HEART_SMALL].render(SCREEN_WIDTH * 25 / 30 - icons[ICON_HEART_SMALL].mWidth * 7 / 8, SCREEN_HEIGHT * 27 / 40 + 201 - icons[ICON_HEART_SMALL].mHeight / 2, gRenderer);

        icons[ICON_MANA_BOTTLE].render(SCREEN_WIDTH / 30 - icons[ICON_HEART_SMALL].mWidth * 6 / 8, SCREEN_HEIGHT / 40 + 195 + icons[ICON_HEALTHBAR].mHeight, gRenderer);
        icons[ICON_MANA_BOTTLE].render(SCREEN_WIDTH / 30 - icons[ICON_HEART_SMALL].mWidth * 6 / 8, SCREEN_HEIGHT * 15 / 40 + 195 + icons[ICON_HEALTHBAR].mHeight, gRenderer);
        icons[ICON_MANA_BOTTLE].render(SCREEN_WIDTH / 30 - icons[ICON_HEART_SMALL].mWidth * 6 / 8, SCREEN_HEIGHT * 27 / 40 + 195 + icons[ICON_HEALTHBAR].mHeight, gRenderer);
        icons[ICON_MANA_BOTTLE].render(SCREEN_WIDTH * 25 / 30 - icons[ICON_HEART_SMALL].mWidth * 6 / 8, SCREEN_HEIGHT / 40 + 195 + icons[ICON_HEALTHBAR].mHeight, gRenderer);
        icons[ICON_MANA_BOTTLE].render(SCREEN_WIDTH * 25 / 30 - icons[ICON_HEART_SMALL].mWidth * 6 / 8, SCREEN_HEIGHT * 15 / 40 + 195 + icons[ICON_HEALTHBAR].mHeight, gRenderer);
        icons[ICON_MANA_BOTTLE].render(SCREEN_WIDTH * 25 / 30 - icons[ICON_HEART_SMALL].mWidth * 6 / 8, SCREEN_HEIGHT * 27 / 40 + 195 + icons[ICON_HEALTHBAR].mHeight, gRenderer);

        SDL_Rect tempRectangle1_1 = {SCREEN_WIDTH / 30,
                                     SCREEN_HEIGHT / 40 + 201,
                                     std::max((int)(1.0 * icons[ICON_HEALTHBAR].mWidth / players[PLAYER_1].playerPokemons[INDEX_POKEMON_ONE].fullHp * players[PLAYER_1].playerPokemons[INDEX_POKEMON_ONE].hp), 0),
                                     icons[ICON_HEALTHBAR].mHeight};

        SDL_Rect tempRectangle1_2 = {SCREEN_WIDTH / 30,
                                     SCREEN_HEIGHT / 40 + 203 + icons[ICON_HEALTHBAR].mHeight,
                                     std::max((int)(1.0 * icons[ICON_HEALTHBAR].mWidth / players[PLAYER_1].playerPokemons[INDEX_POKEMON_ONE].fullMana * players[PLAYER_1].playerPokemons[INDEX_POKEMON_ONE].mana), 0),
                                     icons[ICON_HEALTHBAR].mHeight};

        SDL_Rect tempRectangle2_1 = {SCREEN_WIDTH / 30,
                                     SCREEN_HEIGHT * 15 / 40 + 201,
                                     std::max((int)(1.0 * icons[ICON_HEALTHBAR].mWidth / players[PLAYER_1].playerPokemons[INDEX_POKEMON_TWO].fullHp * players[PLAYER_1].playerPokemons[INDEX_POKEMON_TWO].hp), 0),
                                     icons[ICON_HEALTHBAR].mHeight};

        SDL_Rect tempRectangle2_2 = {SCREEN_WIDTH / 30,
                                     SCREEN_HEIGHT * 15 / 40 + 203 + icons[ICON_HEALTHBAR].mHeight,
                                     std::max((int)(1.0 * icons[ICON_HEALTHBAR].mWidth / players[PLAYER_1].playerPokemons[INDEX_POKEMON_TWO].fullMana * players[PLAYER_1].playerPokemons[INDEX_POKEMON_TWO].mana), 0),
                                     icons[ICON_HEALTHBAR].mHeight};

        SDL_Rect tempRectangle3_1 = {SCREEN_WIDTH / 30,
                                     SCREEN_HEIGHT * 27 / 40 + 201,
                                     std::max((int)(1.0 * icons[ICON_HEALTHBAR].mWidth / players[PLAYER_1].playerPokemons[INDEX_POKEMON_THREE].fullHp * players[PLAYER_1].playerPokemons[INDEX_POKEMON_THREE].hp), 0),
                                     icons[ICON_HEALTHBAR].mHeight};

        SDL_Rect tempRectangle3_2 = {SCREEN_WIDTH / 30,
                                     SCREEN_HEIGHT * 27 / 40 + 203 + icons[ICON_HEALTHBAR].mHeight,
                                     std::max((int)(1.0 * icons[ICON_HEALTHBAR].mWidth / players[PLAYER_1].playerPokemons[INDEX_POKEMON_THREE].fullMana * players[PLAYER_1].playerPokemons[INDEX_POKEMON_THREE].mana), 0),
                                     icons[ICON_HEALTHBAR].mHeight};

        SDL_Rect tempRectangle4_1 = {SCREEN_WIDTH * 25 / 30,
                                     SCREEN_HEIGHT / 40 + 201,
                                     std::max((int)(1.0 * icons[ICON_HEALTHBAR].mWidth / players[PLAYER_2].playerPokemons[INDEX_POKEMON_ONE].fullHp * players[PLAYER_2].playerPokemons[INDEX_POKEMON_ONE].hp), 0),
                                     icons[ICON_HEALTHBAR].mHeight};

        SDL_Rect tempRectangle4_2 = {SCREEN_WIDTH * 25 / 30,
                                     SCREEN_HEIGHT / 40 + 203 + icons[ICON_HEALTHBAR].mHeight,
                                     std::max((int)(1.0 * icons[ICON_HEALTHBAR].mWidth / players[PLAYER_2].playerPokemons[INDEX_POKEMON_ONE].fullMana * players[PLAYER_2].playerPokemons[INDEX_POKEMON_ONE].mana), 0),
                                     icons[ICON_HEALTHBAR].mHeight};

        SDL_Rect tempRectangle5_1 = {SCREEN_WIDTH * 25 / 30,
                                     SCREEN_HEIGHT * 15 / 40 + 201,
                                     std::max((int)(1.0 * icons[ICON_HEALTHBAR].mWidth / players[PLAYER_2].playerPokemons[INDEX_POKEMON_TWO].fullHp * players[PLAYER_2].playerPokemons[INDEX_POKEMON_TWO].hp), 0),
                                     icons[ICON_HEALTHBAR].mHeight};

        SDL_Rect tempRectangle5_2 = {SCREEN_WIDTH * 25 / 30,
                                     SCREEN_HEIGHT * 15 / 40 + 203 + icons[ICON_HEALTHBAR].mHeight,
                                     std::max((int)(1.0 * icons[ICON_HEALTHBAR].mWidth / players[PLAYER_2].playerPokemons[INDEX_POKEMON_TWO].fullMana * players[PLAYER_2].playerPokemons[INDEX_POKEMON_TWO].mana), 0),
                                     icons[ICON_HEALTHBAR].mHeight};

        SDL_Rect tempRectangle6_1 = {SCREEN_WIDTH * 25 / 30,
                                     SCREEN_HEIGHT * 27 / 40 + 201,
                                     std::max((int)(1.0 * icons[ICON_HEALTHBAR].mWidth / players[PLAYER_2].playerPokemons[INDEX_POKEMON_THREE].fullHp * players[PLAYER_2].playerPokemons[INDEX_POKEMON_THREE].hp), 0),
                                     icons[ICON_HEALTHBAR].mHeight};

        SDL_Rect tempRectangle6_2 = {SCREEN_WIDTH * 25 / 30,
                                     SCREEN_HEIGHT * 27 / 40 + 203 + icons[ICON_HEALTHBAR].mHeight,
                                     std::max((int)(1.0 * icons[ICON_HEALTHBAR].mWidth / players[PLAYER_2].playerPokemons[INDEX_POKEMON_THREE].fullMana * players[PLAYER_2].playerPokemons[INDEX_POKEMON_THREE].mana), 0),
                                     icons[ICON_HEALTHBAR].mHeight};

        // hp
        SDL_SetRenderDrawColor(gRenderer, 201, 15, 15, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(gRenderer, &tempRectangle1_1);
        SDL_RenderFillRect(gRenderer, &tempRectangle2_1);
        SDL_RenderFillRect(gRenderer, &tempRectangle3_1);
        SDL_RenderFillRect(gRenderer, &tempRectangle4_1);
        SDL_RenderFillRect(gRenderer, &tempRectangle5_1);
        SDL_RenderFillRect(gRenderer, &tempRectangle6_1);

        // mana
        SDL_SetRenderDrawColor(gRenderer, 0, 66, 225, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(gRenderer, &tempRectangle1_2);
        SDL_RenderFillRect(gRenderer, &tempRectangle2_2);

        SDL_RenderFillRect(gRenderer, &tempRectangle3_2);
        SDL_RenderFillRect(gRenderer, &tempRectangle4_2);
        SDL_RenderFillRect(gRenderer, &tempRectangle5_2);
        SDL_RenderFillRect(gRenderer, &tempRectangle6_2);

        break;
    }
    case STATE_WINNER:
    {
        SDL_Rect tempRect;
        tempRect.w = 200;
        tempRect.h = 200;
        tempRect.x = 0;
        tempRect.y = 0;
        if (currentPlayerNumber == 1)
        {
            addTextToScreenRightAway("PLAYER 1 WINS!", SDL_Color{64, 96, 255, SDL_ALPHA_OPAQUE}, fonts[FONT_BATTLE_64], gRenderer, SCREEN_WIDTH * 27 / 64, 0);
        }
        else
        {
            addTextToScreenRightAway("PLAYER 2 WINS!", SDL_Color{255, 21, 21, SDL_ALPHA_OPAQUE}, fonts[FONT_BATTLE_64], gRenderer, SCREEN_WIDTH * 27 / 64, 0);
        }

        addTextToScreenRightAway("Press enter to continue", SDL_Color{0, 0, 0, SDL_ALPHA_OPAQUE}, fonts[FONT_BATTLE_64], gRenderer, SCREEN_WIDTH * 24 / 64, SCREEN_HEIGHT - 130);

        if (currentPlayer->countAlivePokemons == 1)
        {
            int ind;
            for (int i = INDEX_POKEMON_ONE; i <= INDEX_POKEMON_THREE; i++)
            {
                if (currentPlayer->playerPokemons[i].isAlive)
                {
                    ind = i;
                }
            }

            currentPlayer->playerPokemons[ind].texture.render(SCREEN_WIDTH / 2 - currentPlayer->playerPokemons[ind].texture.mWidth / 2,
                                                              SCREEN_HEIGHT / 2 - currentPlayer->playerPokemons[ind].texture.mHeight / 2, gRenderer);

            icons[ICON_CROWN].render(SCREEN_WIDTH / 2 - currentPlayer->playerPokemons[ind].texture.mWidth / 2,
                                     SCREEN_HEIGHT / 2 - currentPlayer->playerPokemons[ind].texture.mHeight / 2 - icons[ICON_CROWN].mHeight * 11 / 14, gRenderer);
        }
        else if (currentPlayer->countAlivePokemons == 2)
        {
            int ind1 = -1, ind2 = -1;
            for (int i = INDEX_POKEMON_ONE; i <= INDEX_POKEMON_THREE; i++)
            {
                if (currentPlayer->playerPokemons[i].isAlive && ind1 == -1)
                {
                    ind1 = i;
                }
                else if (currentPlayer->playerPokemons[i].isAlive && ind1 != -1)
                {
                    ind2 = i;
                }
            }

            currentPlayer->playerPokemons[ind1].texture.render(SCREEN_WIDTH * 6 / 16 - currentPlayer->playerPokemons[ind1].texture.mWidth / 2,
                                                               SCREEN_HEIGHT / 2 - currentPlayer->playerPokemons[ind1].texture.mHeight / 2, gRenderer);

            currentPlayer->playerPokemons[ind2].texture.render(SCREEN_WIDTH * 10 / 16 - currentPlayer->playerPokemons[ind1].texture.mWidth / 2,
                                                               SCREEN_HEIGHT / 2 - currentPlayer->playerPokemons[ind1].texture.mHeight / 2, gRenderer);

            icons[ICON_CROWN].render(SCREEN_WIDTH * 6 / 16 - currentPlayer->playerPokemons[ind1].texture.mWidth / 2,
                                     SCREEN_HEIGHT / 2 - currentPlayer->playerPokemons[ind1].texture.mHeight / 2 - icons[ICON_CROWN].mHeight * 11 / 14, gRenderer);

            icons[ICON_CROWN].render(SCREEN_WIDTH * 10 / 16 - currentPlayer->playerPokemons[ind1].texture.mWidth / 2,
                                     SCREEN_HEIGHT / 2 - currentPlayer->playerPokemons[ind1].texture.mHeight / 2 - icons[ICON_CROWN].mHeight * 11 / 14, gRenderer);
        }
        else if (currentPlayer->countAlivePokemons == 3)
        {
            currentPlayer->playerPokemons[INDEX_POKEMON_ONE].texture.render(SCREEN_WIDTH * 8 / 16 - currentPlayer->playerPokemons[INDEX_POKEMON_ONE].texture.mWidth / 2,
                                                                            SCREEN_HEIGHT / 2 - currentPlayer->playerPokemons[INDEX_POKEMON_ONE].texture.mHeight / 2, gRenderer);

            currentPlayer->playerPokemons[INDEX_POKEMON_TWO].texture.render(SCREEN_WIDTH * 4 / 16 - currentPlayer->playerPokemons[INDEX_POKEMON_ONE].texture.mWidth / 2,
                                                                            SCREEN_HEIGHT / 2 - currentPlayer->playerPokemons[INDEX_POKEMON_ONE].texture.mHeight / 2, gRenderer);

            currentPlayer->playerPokemons[INDEX_POKEMON_THREE].texture.render(SCREEN_WIDTH * 12 / 16 - currentPlayer->playerPokemons[INDEX_POKEMON_ONE].texture.mWidth / 2,
                                                                              SCREEN_HEIGHT / 2 - currentPlayer->playerPokemons[INDEX_POKEMON_ONE].texture.mHeight / 2, gRenderer);

            icons[ICON_CROWN].render(SCREEN_WIDTH * 8 / 16 - currentPlayer->playerPokemons[INDEX_POKEMON_ONE].texture.mWidth / 2,
                                     SCREEN_HEIGHT / 2 - currentPlayer->playerPokemons[INDEX_POKEMON_ONE].texture.mHeight / 2 - icons[ICON_CROWN].mHeight * 11 / 14, gRenderer);

            icons[ICON_CROWN].render(SCREEN_WIDTH * 4 / 16 - currentPlayer->playerPokemons[INDEX_POKEMON_ONE].texture.mWidth / 2,
                                     SCREEN_HEIGHT / 2 - currentPlayer->playerPokemons[INDEX_POKEMON_ONE].texture.mHeight / 2 - icons[ICON_CROWN].mHeight * 11 / 14, gRenderer);

            icons[ICON_CROWN].render(SCREEN_WIDTH * 12 / 16 - currentPlayer->playerPokemons[INDEX_POKEMON_ONE].texture.mWidth / 2,
                                     SCREEN_HEIGHT / 2 - currentPlayer->playerPokemons[INDEX_POKEMON_ONE].texture.mHeight / 2 - icons[ICON_CROWN].mHeight * 11 / 14, gRenderer);
        }

        break;
    }

    case STATE_END_GAME:
    {
        SDL_Point point = {texts[TEXT_AUTHOR_NICKNAME].mWidth / 2, texts[TEXT_AUTHOR_NICKNAME].mHeight / 2};
        texts[TEXT_POKEMON_FIGHT].render(SCREEN_WIDTH / 2 - texts[TEXT_POKEMON_FIGHT].mWidth / 2, SCREEN_HEIGHT / 40, gRenderer);
        texts[TEXT_GAME_WAS_MADE].render(SCREEN_WIDTH / 2 - texts[TEXT_GAME_WAS_MADE].mWidth / 2, SCREEN_HEIGHT / 3, gRenderer);
        texts[TEXT_AUTHOR_NICKNAME].render(SCREEN_WIDTH / 2 - texts[TEXT_AUTHOR_NICKNAME].mWidth / 2, SCREEN_HEIGHT * 5 / 8,
                                           gRenderer, NULL, tempAngle, &point, SDL_FLIP_NONE);
        texts[TEXT_AUTHOR_NICKNAME].flashing();
        tempAngle++;
        if (tempAngle > 360)
        {
            tempAngle = 0;
        }
        break;
    }
    }

    SDL_RenderPresent(gRenderer);

    switch (globalState)
    {
    case STATE_START:
    {
        break;
    }
    case STATE_CHOOSE_POKEMONS:
    {
        if (nextState)
        {
            SDL_Delay(2000);
        }
        break;
    }
    case STATE_BATTLE:
    {
        if (nextBattleState && battleState == STATE_BATTLE_CHOOSE_ENEMY_POKEMON)
        {
            SDL_Delay(1000);
        }
        break;
    }
    }
}

void Game::displayPokemonType(Pokemon &pokemon, int xPos, int yPos)
{
    if (pokemon.type == TYPE_FIRE)
    {
        icons[ICON_BALL_FIRE].render(xPos, yPos, gRenderer);
    }
    else if (pokemon.type == TYPE_WATER)
    {
        icons[ICON_BALL_WATER].render(xPos, yPos, gRenderer);
    }
    else if (pokemon.type == TYPE_GRASS)
    {
        icons[ICON_BALL_GRASS].render(xPos, yPos, gRenderer);
    }
}

void Game::displayArrow()
{
    if (battleState == STATE_BATTLE_CHOOSE_POKEMON)
    {
        switch (*playerPokemonIndex)
        {
        case INDEX_POKEMON_ONE:
        {
            if (currentPlayerNumber == 1)
            {
                icons[ICON_ARROW_LEFT].render(SCREEN_WIDTH * 4 / 30,
                                              SCREEN_HEIGHT / 40 - players[PLAYER_1].playerPokemons[INDEX_POKEMON_ONE].nameText.mHeight * 23 / 32 + icons[ICON_ARROW_LEFT].mHeight * 5 / 12 + 120, gRenderer);
            }
            else
            {
                icons[ICON_ARROW_RIGHT].render(SCREEN_WIDTH * 24 / 30,
                                               SCREEN_HEIGHT / 40 - players[PLAYER_1].playerPokemons[INDEX_POKEMON_ONE].nameText.mHeight * 23 / 32 + icons[ICON_ARROW_LEFT].mHeight * 5 / 12 + 120, gRenderer);
            }
            break;
        }
        case INDEX_POKEMON_TWO:
        {
            if (currentPlayerNumber == 1)
            {
                icons[ICON_ARROW_LEFT].render(SCREEN_WIDTH * 4 / 30,
                                              SCREEN_HEIGHT * 15 / 40 - players[PLAYER_1].playerPokemons[INDEX_POKEMON_TWO].nameText.mHeight * 23 / 32 + icons[ICON_ARROW_LEFT].mHeight * 5 / 12 + 120, gRenderer);
            }
            else
            {
                icons[ICON_ARROW_RIGHT].render(SCREEN_WIDTH * 24 / 30,
                                               SCREEN_HEIGHT * 15 / 40 - players[PLAYER_1].playerPokemons[INDEX_POKEMON_TWO].nameText.mHeight * 23 / 32 + icons[ICON_ARROW_LEFT].mHeight * 5 / 12 + 120, gRenderer);
            }
            break;
        }
        case INDEX_POKEMON_THREE:
            if (currentPlayerNumber == 1)
            {
                icons[ICON_ARROW_LEFT].render(SCREEN_WIDTH * 4 / 30,
                                              SCREEN_HEIGHT * 27 / 40 - players[PLAYER_1].playerPokemons[INDEX_POKEMON_THREE].nameText.mHeight * 23 / 32 + icons[ICON_ARROW_LEFT].mHeight * 5 / 12 + 120, gRenderer);
            }
            else
            {
                icons[ICON_ARROW_RIGHT].render(SCREEN_WIDTH * 24 / 30,
                                               SCREEN_HEIGHT * 27 / 40 - players[PLAYER_1].playerPokemons[INDEX_POKEMON_THREE].nameText.mHeight * 23 / 32 + icons[ICON_ARROW_LEFT].mHeight * 5 / 12 + 120, gRenderer);
            }
            break;
        }
    }
    else if (battleState == STATE_BATTLE_CHOOSE_ATTACK)
    {
        icons[ICON_ARROW_RIGHT].render(SCREEN_WIDTH / 4 - icons[ICON_ARROW_RIGHT].mWidth * 9 / 8, SCREEN_HEIGHT * 2 * (curIndexAttack + 1) / 16 + 13, gRenderer);
    }
    else if (battleState == STATE_BATTLE_CHOOSE_ENEMY_POKEMON)
    {
        switch (*anotherPlayerPokemonIndex)
        {
        case INDEX_POKEMON_ONE:
        {
            if (currentPlayerNumber == 1)
            {
                icons[ICON_ARROW_RIGHT].render(SCREEN_WIDTH * 24 / 30,
                                               SCREEN_HEIGHT / 40 - players[PLAYER_1].playerPokemons[INDEX_POKEMON_ONE].nameText.mHeight * 23 / 32 + icons[ICON_ARROW_LEFT].mHeight * 5 / 12 + 120, gRenderer);
            }
            else
            {
                icons[ICON_ARROW_LEFT].render(SCREEN_WIDTH * 4 / 30,
                                              SCREEN_HEIGHT / 40 - players[PLAYER_1].playerPokemons[INDEX_POKEMON_ONE].nameText.mHeight * 23 / 32 + icons[ICON_ARROW_LEFT].mHeight * 5 / 12 + 120, gRenderer);
            }
            break;
        }
        case INDEX_POKEMON_TWO:
        {
            if (currentPlayerNumber == 1)
            {
                icons[ICON_ARROW_RIGHT].render(SCREEN_WIDTH * 24 / 30,
                                               SCREEN_HEIGHT * 15 / 40 - players[PLAYER_1].playerPokemons[INDEX_POKEMON_TWO].nameText.mHeight * 23 / 32 + icons[ICON_ARROW_LEFT].mHeight * 5 / 12 + 120, gRenderer);
            }
            else
            {
                icons[ICON_ARROW_LEFT].render(SCREEN_WIDTH * 4 / 30,
                                              SCREEN_HEIGHT * 15 / 40 - players[PLAYER_1].playerPokemons[INDEX_POKEMON_TWO].nameText.mHeight * 23 / 32 + icons[ICON_ARROW_LEFT].mHeight * 5 / 12 + 120, gRenderer);
            }
            break;
        }
        case INDEX_POKEMON_THREE:
            if (currentPlayerNumber == 1)
            {
                icons[ICON_ARROW_RIGHT].render(SCREEN_WIDTH * 24 / 30,
                                               SCREEN_HEIGHT * 27 / 40 - players[PLAYER_1].playerPokemons[INDEX_POKEMON_THREE].nameText.mHeight * 23 / 32 + icons[ICON_ARROW_LEFT].mHeight * 5 / 12 + 120, gRenderer);
            }
            else
            {
                icons[ICON_ARROW_LEFT].render(SCREEN_WIDTH * 4 / 30,
                                              SCREEN_HEIGHT * 27 / 40 - players[PLAYER_1].playerPokemons[INDEX_POKEMON_THREE].nameText.mHeight * 23 / 32 + icons[ICON_ARROW_LEFT].mHeight * 5 / 12 + 120, gRenderer);
            }
            break;
        }
    }
}

void Game::displayAttacks(Player *&player)
{
    for (int i = 0; i <= ATTACK_FIFTH; i++)
    {
        int tempW = SCREEN_WIDTH / 4 + player->playerPokemons[(int)*playerPokemonIndex].attacks[i].attackNameText.mWidth + icons[ICON_ATTACK].mWidth * 8 / 7;
        int tempH = SCREEN_HEIGHT * (i + 1) * 2 / 16 - 15;
        player->playerPokemons[(int)*playerPokemonIndex].attacks[i].attackNameText.render(SCREEN_WIDTH / 4, SCREEN_HEIGHT * (i + 1) * 2 / 16, gRenderer);
        icons[ICON_ATTACK].render(SCREEN_WIDTH / 4 + player->playerPokemons[(int)*playerPokemonIndex].attacks[i].attackNameText.mWidth,
                                  SCREEN_HEIGHT * (i + 1) * 2 / 16 - 15, gRenderer);
        addTextToScreenRightAway(std::to_string(currentPlayer->playerPokemons[(int)*playerPokemonIndex].attacks[i].attackDamage),
                                 SDL_Color{201, 15, 15, SDL_ALPHA_OPAQUE}, fonts[FONT_BATTLE_64], gRenderer,
                                 SCREEN_WIDTH / 4 + player->playerPokemons[(int)*playerPokemonIndex].attacks[i].attackNameText.mWidth + icons[ICON_ATTACK].mWidth * 8 / 7,
                                 SCREEN_HEIGHT * (i + 1) * 2 / 16 - 15);
        icons[ICON_BOTTLE_BIG].render(tempW + icons[ICON_BOTTLE_BIG].mWidth, tempH - 25, gRenderer);
        addTextToScreenRightAway(std::to_string(currentPlayer->playerPokemons[(int)*playerPokemonIndex].attacks[i].attackWasteMana),
                                 SDL_Color{0, 66, 225, SDL_ALPHA_OPAQUE}, fonts[FONT_BATTLE_64], gRenderer,
                                 tempW + icons[ICON_BOTTLE_BIG].mWidth * 2,
                                 tempH - 25 + icons[ICON_BOTTLE_BIG].mHeight / 4);
    }

    addTextToScreenRightAway("Description of the attack:", SDL_Color{0, 0, 0, SDL_ALPHA_OPAQUE}, fonts[FONT_BATTLE_64], gRenderer,
                             SCREEN_WIDTH / 5, SCREEN_HEIGHT * 12 / 16);

    addTextToScreenRightAway(player->playerPokemons[(int)*playerPokemonIndex].attacks[(int)curIndexAttack].attackDescription,
                             SDL_Color{0, 0, 0, SDL_ALPHA_OPAQUE}, fonts[FONT_BATTLE_48], gRenderer,
                             SCREEN_WIDTH * 9 / 50, SCREEN_HEIGHT * 13 / 16);
}

void Game::crossOutDeadPokemons()
{
    if (!players[PLAYER_1].playerPokemons[INDEX_POKEMON_ONE].isAlive)
    {
        icons[ICON_CROSS].render(SCREEN_WIDTH / 30, SCREEN_HEIGHT / 40, gRenderer);
    }
    if (!players[PLAYER_1].playerPokemons[INDEX_POKEMON_TWO].isAlive)
    {
        icons[ICON_CROSS].render(SCREEN_WIDTH / 30, SCREEN_HEIGHT * 15 / 40, gRenderer);
    }
    if (!players[PLAYER_1].playerPokemons[INDEX_POKEMON_THREE].isAlive)
    {
        icons[ICON_CROSS].render(SCREEN_WIDTH / 30, SCREEN_HEIGHT * 27 / 40, gRenderer);
    }

    if (!players[PLAYER_2].playerPokemons[INDEX_POKEMON_ONE].isAlive)
    {
        icons[ICON_CROSS].render(SCREEN_WIDTH * 25 / 30, SCREEN_HEIGHT / 40, gRenderer);
    }
    if (!players[PLAYER_2].playerPokemons[INDEX_POKEMON_TWO].isAlive)
    {
        icons[ICON_CROSS].render(SCREEN_WIDTH * 25 / 30, SCREEN_HEIGHT * 15 / 40, gRenderer);
    }
    if (!players[PLAYER_2].playerPokemons[INDEX_POKEMON_THREE].isAlive)
    {
        icons[ICON_CROSS].render(SCREEN_WIDTH * 25 / 30, SCREEN_HEIGHT * 27 / 40, gRenderer);
    }
}