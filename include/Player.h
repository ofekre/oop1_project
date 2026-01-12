#pragma once
#include "MovingObject.h"

// Forward declarations
class Wall;
class Floor;
class DiggableFloor;
class Ladder;
class Pole;
class Coin;
class Enemy;

class Player : public MovingObject
{
public:
    Player();
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() override;
    void move(float deltaTime) override;
    void handleInput();
    void dig(bool left);
    void update(float deltaTime);  // Update invincibility timer
    
    // Double dispatch collision
    void processCollision(Collider& other) override { other.handleCollision(*this); }
    
    // Collision handlers for each type
    void handleCollision(Wall& wall) override;
    void handleCollision(Floor& floor) override;
    void handleCollision(DiggableFloor& floor) override;
    void handleCollision(Ladder& ladder) override;
    void handleCollision(Pole& pole) override;
    void handleCollision(Coin& coin) override;
    void handleCollision(Enemy& enemy) override;
    
    // Getters for HUD
    int getLives() const { return m_lives; }
    int getScore() const { return m_score; }
    void addScore(int points) { m_score += points; }
    void loseLife();
    
    // Position management
    void saveStartPosition() { m_startPosition = m_position; }
    void resetToStart();  // Return to start position (on enemy hit)
    
    void reset()  // Full reset for new game
    { 
        m_lives = 3; 
        m_score = 0; 
        m_invincibilityTimer = 0; 
        m_velocity = {0.f, 0.f};
        m_onGround = true;
        m_onLadder = false;
        m_onPole = false;
        if (m_sprite.has_value())
        {
            m_sprite->setPosition(m_position);
        }
    }
    
    // State getters
    bool isOnLadder() const { return m_onLadder; }
    bool isOnPole() const { return m_onPole; }
    bool isInvincible() const { return m_invincibilityTimer > 0; }
    bool needsPositionReset() const { return m_needsReset; }
    void clearResetFlag() { m_needsReset = false; }

private:
    int m_lives = 3;
    int m_score = 0;
    bool m_onLadder = false;
    bool m_onPole = false;
    bool m_onGround = false;
    bool m_needsReset = false;  // Flag to signal Board to reset positions
    float m_invincibilityTimer = 0.f;
    sf::Vector2f m_startPosition;
};