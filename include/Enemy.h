#pragma once
#include "MovingObject.h"

class Enemy : public MovingObject
{
public:
    Enemy();
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() override;
    void move(float deltaTime) override;
    void updateAI();
    
    // Position management
    void saveStartPosition() { m_startPosition = m_position; }
    void resetToStart();
    
    // Double dispatch
    void processCollision(Collider& other) override { other.handleCollision(*this); }

private:
    sf::Vector2f m_startPosition;
};