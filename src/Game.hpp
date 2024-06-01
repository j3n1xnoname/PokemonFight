#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include "Player.hpp"
#include "Pokemon.hpp"
#include "Enum.hpp"
#include "LTimer.hpp"

class Game
{
public:
    SDL_Event event;
    bool quit = false;

    bool init();

    void handleEvent();

    void update();
    void display();

    void updateState();

    void close();

    bool loadAllMedia();
    void fillPokemons();
    bool fillFonts();
    bool fillTexts();
    bool fillIcons();
    bool fillSounds();

    void displayPokemonType(Pokemon &, int, int);

    void addTextToScreenRightAway(std::string, SDL_Color, TTF_Font *&, SDL_Renderer *&, int, int);
    void displayArrow();
    void displayAttacks(Player *&);
    void removeDeadPokemons();
    void damageOpponent();
    void removeInaccessibleAttack();
    void crossOutDeadPokemons();

    void countAlivePokemons();

    LTimer fpsTimer;

private:
    SDL_Renderer *gRenderer = NULL;
    SDL_Window *gWindow = NULL;
    const int SCREEN_WIDTH = 1920;
    const int SCREEN_HEIGHT = 1080;
    const int SCREEN_FPS = 60;
    const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

    bool nextState = false;
    bool nextBattleState = false;

    const int COUNT_POKEMONS = 6;
    const int COUNT_PLAYERS = 2;
    const int COUNT_FONTS = 6;
    const int COUNT_TEXTS = 9;
    const int COUNT_ICONS = 14;
    const int COUNT_CHUNKS = 4;
    const int COUNT_MUSIC = 3;

    const Uint8 *currentKeyState = SDL_GetKeyboardState(NULL);
    unsigned int input = 0;
    unsigned int input_press = 0;

    int globalState = STATE_START;
    int battleState = STATE_BATTLE_CHOOSE_POKEMON;

    std::vector<Player> players;
    std::vector<Pokemon> pokemons;
    std::vector<TTF_Font *> fonts;
    std::vector<LTexture> texts;
    std::vector<LTexture> icons;
    std::vector<Mix_Chunk *> chunks;
    std::vector<Mix_Music *> musics;

    LTimer capTimer;
    int countedFrames = 0;

    int currentPlayerNumber = 1;

    Player *currentPlayer;
    Player *anotherPlayer;

    bool selectedPokemons[6];
    int curIndexPokemon = POKEMON_BLASTOISE;
    int playerOnePokemonIndex = INDEX_POKEMON_ONE;
    int playerTwoPokemonIndex = INDEX_POKEMON_ONE;

    int curIndexAttack = ATTACK_FIRST;

    int *playerPokemonIndex;
    int *anotherPlayerPokemonIndex;

    bool choosedPokemon = false;

    bool makeDamage = false;

    const float attackCoefficents[TYPE_NONE][TYPE_NONE] = {{1.0, 1.5, 0.5},
                                                           {0.5, 1.0, 1.5},
                                                           {1.5, 0.5, 1.0}};

    int tempAngle = 0;
};