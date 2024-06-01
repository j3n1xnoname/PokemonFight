#pragma once
enum GameStates
{
    STATE_START,
    STATE_CHOOSE_POKEMONS,
    STATE_BATTLE,
    STATE_WINNER,
    STATE_END_GAME
};

enum BattleStates
{
    STATE_BATTLE_CHOOSE_POKEMON,
    STATE_BATTLE_CHOOSE_ATTACK,
    STATE_BATTLE_CHOOSE_ENEMY_POKEMON
};

enum PokemonTypes
{
    TYPE_WATER,
    TYPE_FIRE,
    TYPE_GRASS,
    TYPE_NONE
};

enum GameFonts
{
    FONT_BATTLE,
    FONT_48,
    FONT_BATTLE_48,
    FONT_BATTLE_64,
    FONT_SEASRN,
    FONT_TUSJ
};

enum PokemonName
{
    POKEMON_BLASTOISE,
    POKEMON_CACTURNE,
    POKEMON_CAMERUPT,
    POKEMON_CHARIZARD,
    POKEMON_GOLDUCK,
    POKEMON_VENUSAUR
};

enum GamePlayers
{
    PLAYER_1,
    PLAYER_2
};

enum Key
{
    KEY_UP = 1,
    KEY_DOWN = 1 << 1,
    KEY_LEFT = 1 << 2,
    KEY_RIGHT = 1 << 3,
    KEY_ENTER = 1 << 4
};

enum GameTexts
{
    TEXT_PRESS_ENTER,
    TEXT_WATER,
    TEXT_GRASS,
    TEXT_FIRE,
    TEXT_PLAYER_ONE,
    TEXT_PLAYER_TWO,
    TEXT_POKEMON_FIGHT,
    TEXT_GAME_WAS_MADE,
    TEXT_AUTHOR_NICKNAME
};

enum PlayerPokemonIndexes
{
    INDEX_POKEMON_ONE,
    INDEX_POKEMON_TWO,
    INDEX_POKEMON_THREE
};

enum GameIcons
{
    ICON_ARROW_DOWN,
    ICON_HEART_BIG,
    ICON_BOTTLE_BIG,
    ICON_BALL_WATER,
    ICON_BALL_FIRE,
    ICON_BALL_GRASS,
    ICON_HEALTHBAR,
    ICON_HEART_SMALL,
    ICON_MANA_BOTTLE,
    ICON_ARROW_LEFT,
    ICON_ARROW_RIGHT,
    ICON_ATTACK,
    ICON_CROSS,
    ICON_CROWN
};

enum AttackIndexes
{
    ATTACK_FIRST,
    ATTACK_SECOND,
    ATTACK_THIRD,
    ATTACK_FOURTH,
    ATTACK_FIFTH
};

enum GAME_CHUNKS
{
    CHUNK_ENTER,
    CHUNK_PRESS_ARROW,
    CHUNK_PRESS_ENTER_MENU,
    CHUNK_HIT
};

enum GAME_MUSICS
{
    MUSIC_BATTLE,
    MUSIC_WINNER,
    MUSIC_END
};