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

    gWindow = SDL_CreateWindow("Pokemon Fight", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_HEIGHT, SCREEN_WIDTH, SDL_WINDOW_SHOWN);
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
}