#include "Game.hpp"

bool Game::init()
{
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) < 0)
    {
        printf("SDL fails to initialize. SDL_Error: %s\n", SDL_GetError());
        success = false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        printf("SDL_image fails to initialize. IMG_Error: %s\n", IMG_GetError());
        success = false;
    }

    if (TTF_Init() < 0)
    {
        printf("SDL_ttf fails to initialize. TTF_Error: %s\n", TTF_GetError());
        success = false;
    }

    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_WAVPACK) < 0)
    {
        printf("SDL_mixer fails to initialize. IMG_Error: %s\n", Mix_GetError());
        success = false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not open audio! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    gWindow = SDL_CreateWindow("Pokemon Fight", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL)
    {
        printf("SDL failed to create window. SDL_Error: %s\n", SDL_GetError());
        success = false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED || SDL_RENDERER_PRESENTVSYNC);
    if (gRenderer == NULL)
    {
        printf("SDL failed to create a renderer. SDL_Error: %s\n", SDL_GetError());
        success = false;
    }

    if (SDL_RenderSetLogicalSize(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT) < 0)
    {
        printf("SDL failed to set logical size. SDL_Error: %s\n", SDL_GetError());
        success = false;
    }

    players.resize(COUNT_PLAYERS);
    pokemons.resize(COUNT_POKEMONS);
    fonts.resize(COUNT_FONTS);
    texts.resize(COUNT_TEXTS);
    icons.resize(COUNT_ICONS);
    players[PLAYER_1].playerPokemons.resize(3);
    players[PLAYER_2].playerPokemons.resize(3);
    chunks.resize(COUNT_CHUNKS);
    musics.resize(COUNT_MUSIC);

    for (int i = 0; i < COUNT_POKEMONS; i++)
    {
        pokemons[i].attacks.resize(5);
        selectedPokemons[i] = false;
    }

    if (!loadAllMedia())
    {
        success = false;
    }
    fillPokemons();
    if (!fillTexts())
    {
        success = false;
    }

    currentPlayer = &players[PLAYER_1];
    anotherPlayer = &players[PLAYER_2];
    playerPokemonIndex = &playerOnePokemonIndex;
    anotherPlayerPokemonIndex = &playerTwoPokemonIndex;

    return success;
}

void Game::close()
{
    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    for (int i = 0; i < COUNT_FONTS; i++)
    {
        if (fonts[i] != NULL)
        {
            TTF_CloseFont(fonts[i]);
            fonts[i] = NULL;
        }
    }

    for (int i = 0; i < COUNT_TEXTS; i++)
    {
        if (!texts[i].isEmpty())
        {
            texts[i].free();
        }
    }

    for (int i = 0; i < COUNT_ICONS; i++)
    {
        if (!icons[i].isEmpty())
        {
            icons[i].free();
        }
    }

    for (int i = 0; i < COUNT_CHUNKS; i++)
    {
        Mix_FreeChunk(chunks[i]);
        chunks[i] = NULL;
    }

    for (int i = 0; i < COUNT_MUSIC; i++)
    {
        Mix_FreeMusic(musics[i]);
        musics[i] = NULL;
    }

    SDL_Quit();
    IMG_Quit();
    Mix_Quit();
    TTF_Quit();
}

void Game::handleEvent()
{
    if (event.type == SDL_QUIT)
    {
        quit = true;
    }

    unsigned int buffer = input;
    input = 0;
    if (currentKeyState[SDL_SCANCODE_RETURN])
    {
        input |= KEY_ENTER;
    }
    else if (currentKeyState[SDL_SCANCODE_LEFT])
    {
        input |= KEY_LEFT;
    }
    else if (currentKeyState[SDL_SCANCODE_RIGHT])
    {
        input |= KEY_RIGHT;
    }
    else if (currentKeyState[SDL_SCANCODE_UP])
    {
        input |= KEY_UP;
    }
    else if (currentKeyState[SDL_SCANCODE_DOWN])
    {
        input |= KEY_DOWN;
    }
    input_press = input & ~buffer;

    switch (globalState)
    {
    case STATE_START:
    {
        if (input_press & KEY_ENTER)
        {
            nextState = true;
            Mix_PlayChannel(-1, chunks[CHUNK_ENTER], 0);
        }

        break;
    }

    case STATE_CHOOSE_POKEMONS:
    {
        if (input_press & KEY_ENTER && !selectedPokemons[(int)curIndexPokemon])
        {
            selectedPokemons[(int)curIndexPokemon] = true;
            currentPlayer->playerPokemons[(int)*playerPokemonIndex] = pokemons[(int)curIndexPokemon];
            *playerPokemonIndex = (PlayerPokemonIndexes)((int)*playerPokemonIndex + 1);
            choosedPokemon = true;

            if (choosedPokemon)
            {
                pokemons[(int)curIndexPokemon].texture.alpha = 100;
                pokemons[(int)curIndexPokemon].texture.setAlphaMod();
            }

            Mix_PlayChannel(-1, chunks[CHUNK_PRESS_ENTER_MENU], 0);
        }
        if (input_press & KEY_RIGHT)
        {
            curIndexPokemon = (PokemonName)((int)curIndexPokemon + 1);
            Mix_PlayChannel(-1, chunks[CHUNK_PRESS_ARROW], 0);
        }
        if (input_press & KEY_LEFT)
        {

            curIndexPokemon = (PokemonName)((int)curIndexPokemon - 1);
            Mix_PlayChannel(-1, chunks[CHUNK_PRESS_ARROW], 0);
        }

        int tempChoosedPokemons = 0;
        for (int i = 0; i < COUNT_POKEMONS; i++)
        {
            if (selectedPokemons[i])
            {
                tempChoosedPokemons++;
            }
        }

        if (tempChoosedPokemons >= 6)
        {
            nextState = true;
        }

        if ((int)curIndexPokemon < 0)
        {
            curIndexPokemon = (PokemonName)0;
        }
        if ((int)curIndexPokemon >= 6)
        {

            curIndexPokemon = (PokemonName)5;
        }
        break;
    }

    case STATE_BATTLE:
    {
        if (Mix_PlayingMusic() == 0)
        {
            Mix_PlayMusic(musics[MUSIC_BATTLE], -1);
        }

        if (input_press & KEY_ENTER)
        {
            Mix_PlayChannel(-1, chunks[CHUNK_PRESS_ENTER_MENU], 0);
            nextBattleState = true;
            switch (battleState)
            {
            case STATE_BATTLE_CHOOSE_POKEMON:
            {
                if (!currentPlayer->playerPokemons[(int)*playerPokemonIndex].isAlive)
                {
                    nextBattleState = false;
                }
                break;
            }

            case STATE_BATTLE_CHOOSE_ATTACK:
            {
                if (currentPlayer->playerPokemons[(int)*playerPokemonIndex].attacks[(int)curIndexAttack].attackWasteMana > currentPlayer->playerPokemons[(int)*playerPokemonIndex].mana)
                {
                    nextBattleState = false;
                }
                break;
            }

            case STATE_BATTLE_CHOOSE_ENEMY_POKEMON:
            {
                if (!anotherPlayer->playerPokemons[(int)*anotherPlayerPokemonIndex].isAlive)
                {
                    nextBattleState = false;
                }
                else
                {
                    Mix_PlayChannel(-1, chunks[CHUNK_HIT], 0);
                    makeDamage = true;
                }

                break;
            }
            }
        }
        if (input_press & KEY_UP)
        {
            Mix_PlayChannel(-1, chunks[CHUNK_PRESS_ARROW], 0);
            switch (battleState)
            {
            case STATE_BATTLE_CHOOSE_POKEMON:
            {
                *playerPokemonIndex = (PlayerPokemonIndexes)((int)*playerPokemonIndex - 1);
                break;
            }
            case STATE_BATTLE_CHOOSE_ATTACK:
            {
                curIndexAttack = (AttackIndexes)((int)curIndexAttack - 1);
                break;
            }
            case STATE_BATTLE_CHOOSE_ENEMY_POKEMON:
            {
                *anotherPlayerPokemonIndex = (PlayerPokemonIndexes)((int)*anotherPlayerPokemonIndex - 1);
                break;
            }
            }
        }
        if (input_press & KEY_DOWN)
        {
            Mix_PlayChannel(-1, chunks[CHUNK_PRESS_ARROW], 0);
            switch (battleState)
            {
            case STATE_BATTLE_CHOOSE_POKEMON:
            {
                *playerPokemonIndex = (PlayerPokemonIndexes)((int)*playerPokemonIndex + 1);
                break;
            }
            case STATE_BATTLE_CHOOSE_ATTACK:
            {
                curIndexAttack = (AttackIndexes)((int)curIndexAttack + 1);
                break;
            }
            case STATE_BATTLE_CHOOSE_ENEMY_POKEMON:
            {
                *anotherPlayerPokemonIndex = (PlayerPokemonIndexes)((int)*anotherPlayerPokemonIndex + 1);
                break;
            }
            }
        }

        if (*playerPokemonIndex > INDEX_POKEMON_THREE)
        {
            *playerPokemonIndex = INDEX_POKEMON_THREE;
        }
        if (*playerPokemonIndex < INDEX_POKEMON_ONE)
        {
            *playerPokemonIndex = INDEX_POKEMON_ONE;
        }
        if (curIndexAttack < ATTACK_FIRST)
        {
            curIndexAttack = ATTACK_FIRST;
        }
        if (curIndexAttack > ATTACK_FIFTH)
        {
            curIndexAttack = ATTACK_FIFTH;
        }
        if (*anotherPlayerPokemonIndex > INDEX_POKEMON_THREE)
        {
            *anotherPlayerPokemonIndex = INDEX_POKEMON_THREE;
        }
        if (*anotherPlayerPokemonIndex < INDEX_POKEMON_ONE)
        {
            *anotherPlayerPokemonIndex = INDEX_POKEMON_ONE;
        }

        break;
    }

    case STATE_WINNER:
    {

        if (input_press & KEY_ENTER)
        {
            nextState = true;
        }
        break;
    }
    case STATE_END_GAME:
    {
        if (Mix_PlayingMusic() == 0)
        {
            Mix_PlayMusic(musics[MUSIC_END], -1);
        }
        break;
    }
    }
}

void Game::update()
{
    switch (globalState)
    {
    case STATE_START:
    {
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 128, SDL_ALPHA_OPAQUE);
        texts[TEXT_PRESS_ENTER].flashing();
        SDL_RenderClear(gRenderer);

        break;
    }

    case STATE_CHOOSE_POKEMONS:
    {
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(gRenderer);
        break;
    }

    case STATE_BATTLE:
    {
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(gRenderer);
        if (battleState == STATE_BATTLE_CHOOSE_ATTACK)
        {
            removeInaccessibleAttack();
        }
        if (makeDamage)
        {
            damageOpponent();
        }
        removeDeadPokemons();

        break;
    }
    case STATE_WINNER:
    {
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(gRenderer);
        if (Mix_PlayingMusic() == 0)
        {
            Mix_PlayMusic(musics[MUSIC_WINNER], -1);
        }
        break;
    }
    case STATE_END_GAME:
    {
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(gRenderer);
        break;
    }
    }
}

void Game::display()
{
    switch (globalState)
    {
    case STATE_START:
    {
        texts[TEXT_PRESS_ENTER].render(SCREEN_WIDTH / 2 - texts[TEXT_PRESS_ENTER].mWidth / 2, SCREEN_HEIGHT / 2 - texts[TEXT_PRESS_ENTER].mHeight / 2, gRenderer);
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

void Game::updateState()
{

    switch (globalState)
    {
    case STATE_START:
    {
        if (nextState)
        {
            texts[TEXT_PRESS_ENTER].alpha = 255;
            texts[TEXT_PRESS_ENTER].setAlphaMod();
        }
        break;
    }
    case STATE_CHOOSE_POKEMONS:
    {
        if (choosedPokemon)
        {
            choosedPokemon = false;
            if (currentPlayerNumber == 1)
            {
                playerPokemonIndex = &playerTwoPokemonIndex;
                currentPlayerNumber = 2;
            }
            else
            {
                playerPokemonIndex = &playerOnePokemonIndex;
                currentPlayerNumber = 1;
            }
            std::swap(currentPlayer, anotherPlayer);
        }
        if (nextState)
        {
            currentPlayer = &players[PLAYER_1];
            anotherPlayer = &players[PLAYER_2];
            currentPlayerNumber = 1;
            for (int i = 0; i < COUNT_POKEMONS; i++)
            {
                pokemons[i].texture.alpha = 255;
                pokemons[i].texture.setAlphaMod();
            }
            choosedPokemon = false;

            playerOnePokemonIndex = INDEX_POKEMON_ONE;
            playerTwoPokemonIndex = INDEX_POKEMON_ONE;
            playerPokemonIndex = &playerOnePokemonIndex;
        }
        break;
    }

    case STATE_BATTLE:
    {

        if (battleState == STATE_BATTLE_CHOOSE_POKEMON)
        {
            int tempDeadPokemon1 = 0;
            int tempDeadPokemon2 = 0;
            for (int i = INDEX_POKEMON_ONE; i <= INDEX_POKEMON_THREE; i++)
            {
                if (!currentPlayer->playerPokemons[i].isAlive)
                {
                    tempDeadPokemon1++;
                }
                if (!anotherPlayer->playerPokemons[i].isAlive)
                {
                    tempDeadPokemon2++;
                }
            }

            if (tempDeadPokemon1 == 3)
            {
                currentPlayer->isAlive = false;
                nextState = true;
            }

            if (tempDeadPokemon2 == 3)
            {
                nextState = true;
                anotherPlayer->isAlive = false;
            }
        }

        if (battleState == STATE_BATTLE_CHOOSE_ATTACK && nextBattleState)
        {
            for (int i = ATTACK_FIRST; i <= ATTACK_FIFTH; i++)
            {

                currentPlayer->playerPokemons[(int)*playerPokemonIndex].attacks[i].attackNameText.alpha = 255;
                currentPlayer->playerPokemons[(int)*playerPokemonIndex].attacks[i].attackNameText.setAlphaMod();
            }
        }

        if ((battleState == STATE_BATTLE_CHOOSE_ENEMY_POKEMON && nextBattleState) || (battleState == STATE_BATTLE_CHOOSE_ATTACK && curIndexAttack == ATTACK_FIFTH && nextBattleState))
        {
            for (int i = INDEX_POKEMON_ONE; i <= INDEX_POKEMON_THREE; i++)
            {
                if (currentPlayer->playerPokemons[i].isAlive)
                {
                    currentPlayer->playerPokemons[i].hp = std::min(currentPlayer->playerPokemons[i].hp + 5, currentPlayer->playerPokemons[i].fullHp);
                    currentPlayer->playerPokemons[i].mana = std::min(currentPlayer->playerPokemons[i].mana + 10, currentPlayer->playerPokemons[i].fullMana);
                }
                if (anotherPlayer->playerPokemons[i].isAlive)
                {
                    anotherPlayer->playerPokemons[i].hp = std::min(anotherPlayer->playerPokemons[i].hp + 5, anotherPlayer->playerPokemons[i].fullHp);
                    anotherPlayer->playerPokemons[i].mana = std::min(anotherPlayer->playerPokemons[i].mana + 10, anotherPlayer->playerPokemons[i].fullMana);
                }
            }

            battleState = STATE_BATTLE_CHOOSE_POKEMON;
            curIndexAttack = ATTACK_FIRST;
            playerOnePokemonIndex = INDEX_POKEMON_ONE;

            playerTwoPokemonIndex = INDEX_POKEMON_ONE;
            std::swap(playerPokemonIndex, anotherPlayerPokemonIndex);
            nextBattleState = false;

            *playerPokemonIndex = INDEX_POKEMON_ONE;
            *anotherPlayerPokemonIndex = INDEX_POKEMON_ONE;
            std::swap(currentPlayer, anotherPlayer);
            if (currentPlayerNumber == 1)
            {
                currentPlayerNumber = 2;
            }
            else
            {
                currentPlayerNumber = 1;
            }
        }
        makeDamage = false;

        if (nextBattleState)
        {
            nextBattleState = false;
            battleState = (BattleStates)((int)battleState + 1);
        }
        break;
    }
    }

    if (nextState)
    {
        if (globalState == STATE_BATTLE)
        {
            countAlivePokemons();
        }
        globalState = (GameStates)((int)globalState + 1);
        nextState = false;

        if (Mix_PlayingMusic())
        {
            Mix_HaltMusic();
        }
    }

    float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
    if (avgFPS > 2000000)
    {
        avgFPS = 0;
    }
    ++countedFrames;
    int frameTicks = capTimer.getTicks();
    if (frameTicks < SCREEN_TICK_PER_FRAME)
    {
        SDL_Delay(SCREEN_TICK_PER_FRAME - frameTicks);
    }
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

void Game::addTextToScreenRightAway(std::string text, SDL_Color color, TTF_Font *&font, SDL_Renderer *&renderer, int xPos, int yPos)
{
    LTexture texture;
    texture.loadFromRenderedText(text, color, font, renderer);
    texture.render(xPos, yPos, renderer);

    texture.free();
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
        switch (curIndexAttack)
        {
        case ATTACK_FIRST:
        {
            icons[ICON_ARROW_RIGHT].render(SCREEN_WIDTH / 4 - icons[ICON_ARROW_RIGHT].mWidth * 9 / 8, SCREEN_HEIGHT * 2 / 16 + 13, gRenderer);
            break;
        }

        case ATTACK_SECOND:
        {
            icons[ICON_ARROW_RIGHT].render(SCREEN_WIDTH / 4 - icons[ICON_ARROW_RIGHT].mWidth * 9 / 8, SCREEN_HEIGHT * 4 / 16 + 13, gRenderer);
            break;
        }
        case ATTACK_THIRD:
        {
            icons[ICON_ARROW_RIGHT].render(SCREEN_WIDTH / 4 - icons[ICON_ARROW_RIGHT].mWidth * 9 / 8, SCREEN_HEIGHT * 6 / 16 + 13, gRenderer);
            break;
        }
        case ATTACK_FOURTH:
        {
            icons[ICON_ARROW_RIGHT].render(SCREEN_WIDTH / 4 - icons[ICON_ARROW_RIGHT].mWidth * 9 / 8, SCREEN_HEIGHT * 8 / 16 + 13, gRenderer);
            break;
        }
        case ATTACK_FIFTH:
        {
            icons[ICON_ARROW_RIGHT].render(SCREEN_WIDTH / 4 - icons[ICON_ARROW_RIGHT].mWidth * 9 / 8, SCREEN_HEIGHT * 10 / 16 + 13, gRenderer);
            break;
        }
        }
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

    // player->playerPokemons[*playerPokemonIndex].attacks[curIndexAttack].attackDamageText.render(SCREE)
}

void Game::removeDeadPokemons()
{
    for (int i = INDEX_POKEMON_ONE; i <= INDEX_POKEMON_THREE; i++)
    {
        if (!currentPlayer->playerPokemons[i].isAlive)
        {
            currentPlayer->playerPokemons[i].textureBattle.alpha = 100;
            currentPlayer->playerPokemons[i].textureBattle.setAlphaMod();
            currentPlayer->playerPokemons[i].nameText.alpha = 100;
            currentPlayer->playerPokemons[i].nameText.setAlphaMod();
            currentPlayer->playerPokemons[i].hp = 0;
            currentPlayer->playerPokemons[i].mana = 0;
        }
        if (!anotherPlayer->playerPokemons[i].isAlive)
        {
            anotherPlayer->playerPokemons[i].textureBattle.alpha = 100;
            anotherPlayer->playerPokemons[i].textureBattle.setAlphaMod();
            anotherPlayer->playerPokemons[i].nameText.alpha = 100;
            anotherPlayer->playerPokemons[i].nameText.setAlphaMod();
            anotherPlayer->playerPokemons[i].hp = 0;
            anotherPlayer->playerPokemons[i].mana = 0;
        }
    }
}

void Game::damageOpponent()
{
    anotherPlayer->playerPokemons[(int)*anotherPlayerPokemonIndex].hp -= (int)(currentPlayer->playerPokemons[(int)*playerPokemonIndex].attacks[(int)curIndexAttack].attackDamage * attackCoefficents[currentPlayer->playerPokemons[(int)*playerPokemonIndex].type][anotherPlayer->playerPokemons[*anotherPlayerPokemonIndex].type]);
    if (anotherPlayer->playerPokemons[(int)*anotherPlayerPokemonIndex].hp < 0)
    {
        anotherPlayer->playerPokemons[(int)*anotherPlayerPokemonIndex].hp = 0;
        anotherPlayer->playerPokemons[(int)*anotherPlayerPokemonIndex].isAlive = false;
    }

    currentPlayer->playerPokemons[(int)*playerPokemonIndex].mana -= currentPlayer->playerPokemons[(int)*playerPokemonIndex].attacks[(int)curIndexAttack].attackWasteMana;
}

void Game::removeInaccessibleAttack()
{
    for (int i = ATTACK_FIRST; i <= ATTACK_FIFTH; i++)
    {
        if (currentPlayer->playerPokemons[(int)*playerPokemonIndex].attacks[i].attackWasteMana > currentPlayer->playerPokemons[(int)*playerPokemonIndex].mana)
        {
            currentPlayer->playerPokemons[(int)*playerPokemonIndex].attacks[i].attackNameText.alpha = 100;
            currentPlayer->playerPokemons[(int)*playerPokemonIndex].attacks[i].attackNameText.setAlphaMod();
        }
    }
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

void Game::countAlivePokemons()
{

    for (int i = INDEX_POKEMON_ONE; i <= INDEX_POKEMON_THREE; i++)
    {
        if (!currentPlayer->playerPokemons[i].isAlive)
        {
            currentPlayer->countAlivePokemons--;
        }

        if (!anotherPlayer->playerPokemons[i].isAlive)
        {
            anotherPlayer->countAlivePokemons--;
        }
    }

    if (players[PLAYER_1].isAlive)
    {
        currentPlayer = &players[PLAYER_1];
        currentPlayerNumber = 1;
    }
    else
    {
        currentPlayer = &players[PLAYER_2];
        currentPlayerNumber = 2;
    }
}

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