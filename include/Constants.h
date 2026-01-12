#pragma once

namespace GameConstants
{
    // Tile size in pixels
    constexpr float TILE_SIZE = 50.0f;
    
    // HUD settings
    constexpr float HUD_HEIGHT = 30.0f;  // Space for HUD above board
    
    // Window settings
    constexpr unsigned int WINDOW_WIDTH = 1500;
    constexpr unsigned int WINDOW_HEIGHT = 600;
    constexpr unsigned int FRAME_RATE = 60;
    
    // Player settings
    constexpr float PLAYER_SPEED = 200.0f;
    constexpr int STARTING_LIVES = 3;
    
    // Enemy settings
    constexpr float ENEMY_SPEED = 150.0f;
    
    // Physics
    constexpr float GRAVITY = 9500.0f;
    
    // Scoring
    constexpr int COIN_SCORE_MULTIPLIER = 2;    // score = 2 * level
    constexpr int LEVEL_COMPLETE_MULTIPLIER = 50; // score = 50 * level
    
    // Board file characters
    namespace Chars
    {
        constexpr char PLAYER = '@';
        constexpr char ENEMY = '%';
        constexpr char COIN = '*';
        constexpr char WALL = '#';
        constexpr char DIGGABLE_FLOOR = '^';  // Shift+6
        constexpr char LADDER = 'H';
        constexpr char POLE = '-';
        constexpr char EMPTY = ' ';
    }
    
    // Resource paths
    namespace Textures
    {
        constexpr const char* PLAYER = "player";
        constexpr const char* ENEMY = "enemy";
        constexpr const char* COIN = "coin";
        constexpr const char* WALL = "wall";
        constexpr const char* GROUND = "ground";
        constexpr const char* LADDER = "ladder";
        constexpr const char* POLE = "pole";
    }
    
    namespace Fonts
    {
        constexpr const char* MAIN = "arial";
    }
}
