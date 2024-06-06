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

void Game::addTextToScreenRightAway(std::string text, SDL_Color color, TTF_Font *&font, SDL_Renderer *&renderer, int xPos, int yPos)
{
    LTexture texture;
    texture.loadFromRenderedText(text, color, font, renderer);
    texture.render(xPos, yPos, renderer);

    texture.free();
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
