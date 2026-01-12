#include "Board.h"
#include "StaticObject.h"
#include "Resourcemanager.h"
#include "Constants.h"
#include <fstream>

using namespace GameConstants;

void Board::loadFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        return;
    }

    m_levelData.clear();
    
    std::string line;
    std::vector<std::string> currentLevel;
    
    // Read all lines and split by empty lines
    while (std::getline(file, line))
    {
        // Empty line = level separator
        if (line.empty() || (line.size() == 1 && line[0] == '\r'))
        {
            if (!currentLevel.empty())
            {
                m_levelData.push_back(currentLevel);
                currentLevel.clear();
            }
        }
        else
        {
            // Remove trailing \r if present (Windows line endings)
            if (!line.empty() && line.back() == '\r')
            {
                line.pop_back();
            }
            currentLevel.push_back(line);
        }
    }
    
    // Don't forget the last level (no empty line at end)
    if (!currentLevel.empty())
    {
        m_levelData.push_back(currentLevel);
    }
    
    // Load the first level
    m_currentLevel = 0;
    m_player.reset();  // Reset lives and score for fresh start
    if (!m_levelData.empty())
    {
        loadLevel(0);
    }
}

void Board::loadLevel(int levelIndex)
{
    if (levelIndex < 0 || levelIndex >= static_cast<int>(m_levelData.size()))
    {
        return;
    }
    
    m_currentLevel = levelIndex;
    parseLevel(m_levelData[levelIndex]);
}

void Board::nextLevel()
{
    if (hasMoreLevels())
    {
        loadLevel(m_currentLevel + 1);
    }
    else
    {
        // Loop back to first level (infinite loop)
        loadLevel(0);
    }
}

void Board::parseLevel(const std::vector<std::string>& lines)
{
    m_objects.clear();
    m_enemies.clear();
    
    size_t maxCols = 0;
    int row = 0;
    
    for (const auto& line : lines)
    {
        if (line.size() > maxCols)
        {
            maxCols = line.size();
        }
        
        for (size_t col = 0; col < line.size(); ++col)
        {
            char c = line[col];
            float x = static_cast<float>(col) * TILE_SIZE;
            float y = static_cast<float>(row) * TILE_SIZE + HUD_HEIGHT;  // Offset for HUD
            
            switch (c)
            {
                case Chars::WALL:
                {
                    auto wall = std::make_unique<Wall>();
                    wall->setPosition(x, y);
                    m_objects.push_back(std::move(wall));
                    break;
                }
                case Chars::DIGGABLE_FLOOR:
                {
                    auto floor = std::make_unique<DiggableFloor>();
                    floor->setPosition(x, y);
                    m_objects.push_back(std::move(floor));
                    break;
                }
                case Chars::LADDER:
                {
                    auto ladder = std::make_unique<Ladder>();
                    ladder->setPosition(x, y);
                    m_objects.push_back(std::move(ladder));
                    break;
                }
                case Chars::POLE:
                {
                    auto pole = std::make_unique<Pole>();
                    pole->setPosition(x, y);
                    m_objects.push_back(std::move(pole));
                    break;
                }
                case Chars::COIN:
                {
                    auto coin = std::make_unique<Coin>();
                    coin->setPosition(x, y);
                    m_objects.push_back(std::move(coin));
                    break;
                }
                case Chars::PLAYER:
                {
                    m_player.setPosition(x, y);
                    break;
                }
                case Chars::ENEMY:
                {
                    Enemy enemy;
                    enemy.setPosition(x, y);
                    m_enemies.push_back(enemy);
                    break;
                }
            }
        }
        ++row;
    }
    
    // Calculate board dimensions
    m_width = static_cast<float>(maxCols) * TILE_SIZE;
    m_height = static_cast<float>(row) * TILE_SIZE;
    
    // Save starting positions for player and enemies
    m_player.saveStartPosition();
    for (auto& enemy : m_enemies)
    {
        enemy.saveStartPosition();
    }
}

void Board::draw(sf::RenderWindow& window)
{
	for (const auto& obj : m_objects)
    {
        obj->draw(window);
    }
    
    for (auto& enemy : m_enemies)
    {
        enemy.draw(window);
    }
    
    m_player.draw(window);
}

void Board::update(float deltaTime)
{
    m_player.update(deltaTime);  // Update invincibility timer
    
    // Check collisions FIRST to set ground/ladder/pole state
    for (auto& obj : m_objects)
    {
        if (m_player.getBounds().findIntersection(obj->getBounds()))
        {
            obj->processCollision(m_player);  // Double dispatch!
        }
    }
    
    // Now handle input (m_onLadder is correctly set)
    m_player.handleInput();
    
    // Now move (gravity will use correct ground state)
    m_player.move(deltaTime);
    
    // Check collisions again after move to push player out of walls
    for (auto& obj : m_objects)
    {
        if (m_player.getBounds().findIntersection(obj->getBounds()))
        {
            obj->processCollision(m_player);  // Double dispatch!
        }
    }
    
    // Check collisions with enemies
    for (auto& enemy : m_enemies)
    {
        enemy.updateAI();
        enemy.move(deltaTime);
        
        if (m_player.getBounds().findIntersection(enemy.getBounds()))
        {
            enemy.processCollision(m_player);  // Double dispatch!
        }
    }
    
    // Check if player needs position reset (hit enemy)
    if (m_player.needsPositionReset())
    {
        m_player.resetToStart();
        for (auto& enemy : m_enemies)
        {
            enemy.resetToStart();
        }
    }
}

void Board::resetPositions()
{
    // Reload current level to reset positions
    if (m_currentLevel < static_cast<int>(m_levelData.size()))
    {
        parseLevel(m_levelData[m_currentLevel]);
    }
}

void Board::fullReset()
{
    m_currentLevel = 0;
    if (!m_levelData.empty())
    {
        parseLevel(m_levelData[0]);
    }
}

bool Board::isLevelComplete() const
{
    // Level is complete when all coins are collected
    for (const auto& obj : m_objects)
    {
        // Try to cast to Coin and check if collected
        Coin* coin = dynamic_cast<Coin*>(obj.get());
        if (coin && !coin->isCollected())
        {
            return false;  // Still have uncollected coins
        }
    }
    return true;  // All coins collected!
}
