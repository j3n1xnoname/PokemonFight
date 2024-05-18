#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>

class Game
{
public:
    SDL_Event event;

    void init();

private:
    SDL_Renderer *gRenderer = NULL;
    SDL_Window *gWindow = NULL;
    const int SCREEN_WIDTH = 1920;
    const int SCREEN_HEIGHT = 1080;
};