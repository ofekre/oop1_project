#pragma once
#include <vector>
#include <memory>
#include <string>
#include "GameObject.h"
#include "Player.h"
#include "Enemy.h"

class Board
{
public:
    void loadFromFile(const std::string& filename);
    void loadLevel(int levelIndex);  // Load specific level
    void nextLevel();                // Load next level
    void draw(sf::RenderWindow& window);
    void update(float deltaTime);
    void resetPositions();
    void fullReset();
    bool isLevelComplete() const;
    
    // Get board dimensions in pixels
    float getWidth() const { return m_width; }
    float getHeight() const { return m_height; }
    
    // Level info
    int getLevelNumber() const { return m_currentLevel + 1; }  // 1-indexed for display
    int getTotalLevels() const { return static_cast<int>(m_levelData.size()); }
    bool hasMoreLevels() const { return m_currentLevel + 1 < static_cast<int>(m_levelData.size()); }
    
    // Getters for HUD
    int getPlayerScore() const { return m_player.getScore(); }
    int getPlayerLives() const { return m_player.getLives(); }

private:
    void parseLevel(const std::vector<std::string>& lines);
    
    std::vector<std::unique_ptr<GameObject>> m_objects;
    Player m_player;
    std::vector<Enemy> m_enemies;
    
    // Multi-level support
    std::vector<std::vector<std::string>> m_levelData;  // All levels stored as lines
    int m_currentLevel = 0;
    
    float m_width = 0.f;
    float m_height = 0.f;
};