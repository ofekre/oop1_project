#pragma once
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Menu.h"
#include "HUD.h"
#include "ResourceManager.h"

enum class GameState
{
    Menu,
    Playing,
    Victory,
    GameOver
};

class GameController
{
public:
    void run();

private:
    void handleEvents();
    void update();
    void render();
    void loadNextLevel();
    void showEndScreen(const std::string& message, sf::Color color);
    void resetToMenu();

    sf::RenderWindow m_window;
    Board m_board;
    Menu m_menu;
    HUD m_hud;
    sf::Clock m_clock;
    sf::Clock m_gameTimer;      // Timer for HUD display
    sf::Clock m_endScreenTimer; // Timer for end screen display
    
    GameState m_state = GameState::Menu;
    std::string m_endMessage;
    sf::Color m_endMessageColor;
};