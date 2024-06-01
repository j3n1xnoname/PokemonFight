#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "string"

class LTexture
{
public:
    LTexture();
    ~LTexture();

    bool loadFromFile(std::string path, SDL_Renderer *gRenderer);

    bool loadFromRenderedText(std::string textureText, SDL_Color textColor, TTF_Font *gFont, SDL_Renderer *gRenderer);

    void free();

    void flashing();

    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    void render(int x, int y, SDL_Renderer *gRenderer, SDL_Rect *clip = NULL, double angle = 0.0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

    void setAlphaMod();

    // int getWidth() { return mWidth; }
    // int getHeight() { return mHeight; }

    bool isEmpty() { return mTexture == NULL; }

    Uint8 alpha;
    int mWidth;
    int mHeight;

    SDL_Texture *mTexture;

private:
    // SDL_Texture *mTexture;
};