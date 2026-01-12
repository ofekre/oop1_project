#include "Hud.h"
#include "Resourcemanager.h"
#include <string>

void HUD::draw(sf::RenderWindow& window)
{
    sf::Font& font = ResourceManager::instance().getFont("arial");
    
    sf::Text levelText(font);
    levelText.setString(m_levelStr);
    levelText.setCharacterSize(16);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition({10.f, 5.f});
    window.draw(levelText);
    
    sf::Text scoreText(font);
    scoreText.setString(m_scoreStr);
    scoreText.setCharacterSize(16);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition({150.f, 5.f});
    window.draw(scoreText);
    
    sf::Text livesText(font);
    livesText.setString(m_livesStr);
    livesText.setCharacterSize(16);
    livesText.setFillColor(sf::Color::White);
    livesText.setPosition({300.f, 5.f});
    window.draw(livesText);
    
    sf::Text timerText(font);
    timerText.setString(m_timerStr);
    timerText.setCharacterSize(16);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition({450.f, 5.f});
    window.draw(timerText);
}

void HUD::update(int level, int score, int lives, float time)
{
    m_levelStr = "Level: " + std::to_string(level);
    m_scoreStr = "Score: " + std::to_string(score);
    m_livesStr = "Lives: " + std::to_string(lives);
    m_timerStr = "Time: " + std::to_string(static_cast<int>(time));
}
