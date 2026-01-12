#include "Gamecontroller.h"
#include "Constants.h"

using namespace GameConstants;

void GameController::run()
{
    // Create initial window (will resize after loading level)
    m_window.create(sf::VideoMode({800, 400}), "Lode Runner");
    m_window.setFramerateLimit(FRAME_RATE);
    
    m_state = GameState::Menu;
    
    while (m_window.isOpen())
    {
        handleEvents();
        update();
        render();
    }
}

void GameController::handleEvents()
{
    while (const auto event = m_window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            m_window.close();
        }
        
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->code == sf::Keyboard::Key::Enter)
            {
                if (m_state == GameState::Menu)
                {
                    m_state = GameState::Playing;
                    loadNextLevel();
                }
            }
            if (keyPressed->code == sf::Keyboard::Key::Escape)
            {
                m_window.close();
            }
        }
    }
}

void GameController::update()
{
    // Handle end screen timeout (Victory and GameOver return to menu after delay)
    if (m_state == GameState::GameOver || m_state == GameState::Victory)
    {
        if (m_endScreenTimer.getElapsedTime().asSeconds() > 3.0f)
        {
            resetToMenu();
        }
        return;
    }
    
    if (m_state != GameState::Playing)
    {
        return;
    }
    
    float deltaTime = m_clock.restart().asSeconds();
    
    if (deltaTime > 0.1f)
    {
        deltaTime = 0.1f;
    }
    
    m_board.update(deltaTime);
    
    // Check for game over (no lives left)
    if (m_board.getPlayerLives() <= 0)
    {
        m_state = GameState::GameOver;
        m_endMessage = "GAME OVER";
        m_endMessageColor = sf::Color::Red;
        m_endScreenTimer.restart();
        return;
    }
    
    // Check if level is complete (all coins collected)
    if (m_board.isLevelComplete())
    {
        if (m_board.hasMoreLevels())
        {
            // Go to next level
            m_board.nextLevel();
            m_gameTimer.restart();
            
            // Resize window to fit new level
            unsigned int newWidth = static_cast<unsigned int>(m_board.getWidth());
            unsigned int newHeight = static_cast<unsigned int>(m_board.getHeight() + GameConstants::HUD_HEIGHT);
            m_window.setSize({newWidth, newHeight});
            m_window.setView(sf::View(sf::FloatRect({0, 0}, {static_cast<float>(newWidth), static_cast<float>(newHeight)})));
        }
        else
        {
            // All levels complete - Victory!
            m_state = GameState::Victory;
            m_endMessage = "VICTORY!";
            m_endMessageColor = sf::Color::Green;
            m_endScreenTimer.restart();
        }
    }
    
    // Update HUD with current game state
    float elapsedTime = m_gameTimer.getElapsedTime().asSeconds();
    m_hud.update(
        m_board.getLevelNumber(),
        m_board.getPlayerScore(),
        m_board.getPlayerLives(),
        elapsedTime
    );
}

void GameController::render()
{
    m_window.clear(sf::Color::Black);
    
    if (m_state == GameState::Playing || m_state == GameState::Victory || m_state == GameState::GameOver)
    {
        m_board.draw(m_window);
        m_hud.draw(m_window);
        
        // Draw end screen overlay
        if (m_state == GameState::Victory || m_state == GameState::GameOver)
        {
            showEndScreen(m_endMessage, m_endMessageColor);
        }
    }
    else
    {
        m_menu.draw(m_window);
    }
    
    m_window.display();
}

void GameController::loadNextLevel()
{
    m_board.loadFromFile("resources/Board.txt");
    
    // Resize window to fit board + HUD space
    unsigned int width = static_cast<unsigned int>(m_board.getWidth());
    unsigned int height = static_cast<unsigned int>(m_board.getHeight() + HUD_HEIGHT);
    
    m_window.setSize({width, height});
    
    // Also update the view to match
    sf::View view(sf::FloatRect({0.f, 0.f}, {static_cast<float>(width), static_cast<float>(height)}));
    m_window.setView(view);
    
    m_gameTimer.restart();
    m_clock.restart();
}

void GameController::showEndScreen(const std::string& message, sf::Color color)
{
    // Draw semi-transparent overlay
    sf::RectangleShape overlay({m_board.getWidth(), m_board.getHeight() + HUD_HEIGHT});
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    m_window.draw(overlay);
    
    // Draw message
    sf::Text text(ResourceManager::instance().getFont(Fonts::MAIN), message, 72);
    text.setFillColor(color);
    
    // Center the text
    sf::FloatRect textBounds = text.getGlobalBounds();
    float x = (m_board.getWidth() - textBounds.size.x) / 2.0f;
    float y = (m_board.getHeight() + HUD_HEIGHT - textBounds.size.y) / 2.0f;
    text.setPosition({x, y});
    
    m_window.draw(text);
}

void GameController::resetToMenu()
{
    m_state = GameState::Menu;
    
    // Reload all levels from file for fresh start
    m_board.loadFromFile("resources/Board.txt");
    
    // Reset window size for menu
    m_window.setSize({800, 400});
    m_window.setView(sf::View(sf::FloatRect({0, 0}, {800, 400})));
}
