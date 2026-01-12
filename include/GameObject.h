#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include "Collider.h"

class GameObject : public Collider
{
public:
    virtual ~GameObject() = default;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual sf::FloatRect getBounds() = 0;
    void setPosition(float x, float y);
    
    // Default implementation - does nothing, derived classes override
    void processCollision(Collider& other) override {}

protected:
    void setTexture(const sf::Texture& texture);
    std::optional<sf::Sprite> m_sprite;
    sf::Vector2f m_position;
};