#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#include "Game.hpp"

Game *game = new Game();

int main()
{

    std::srand(std::time(0));
    if (!game->init())
    {
        printf("Failed to init a game!\n");
    }

    game->fpsTimer.start();

    while (!game->quit)
    {
        while (SDL_PollEvent(&game->event) != 0)
        {
            game->handleEvent();
        }

        game->update();

        game->display();

        game->updateState();
    }

    game->close();
    return 0;
}
