#include "StaticObject.h"
#include "Resourcemanager.h"
#include "Constants.h"

using namespace GameConstants;

// ===== Wall =====
Wall::Wall()
{
    setTexture(ResourceManager::instance().getTexture(Textures::WALL));
}

void Wall::draw(sf::RenderWindow& window)
{
    if (m_sprite.has_value())
    {
        window.draw(*m_sprite);
    }
}

sf::FloatRect Wall::getBounds()
{
    if (m_sprite.has_value())
    {
        return m_sprite->getGlobalBounds();
    }
    return sf::FloatRect({m_position.x, m_position.y}, {TILE_SIZE, TILE_SIZE});
}

// ===== BaseFloor (abstract) =====
void BaseFloor::draw(sf::RenderWindow& window)
{
    if (m_sprite.has_value())
    {
        window.draw(*m_sprite);
    }
}

sf::FloatRect BaseFloor::getBounds()
{
    if (m_sprite.has_value())
    {
        return m_sprite->getGlobalBounds();
    }
    return sf::FloatRect({m_position.x, m_position.y}, {TILE_SIZE, TILE_SIZE});
}

// ===== Floor =====
Floor::Floor()
{
    setTexture(ResourceManager::instance().getTexture(Textures::GROUND));
}

// ===== DiggableFloor =====
DiggableFloor::DiggableFloor()
{
    setTexture(ResourceManager::instance().getTexture(Textures::GROUND));
}

void DiggableFloor::dig()
{
    m_isDigged = true;
}

void DiggableFloor::restore()
{
    m_isDigged = false;
}

bool DiggableFloor::isDigged() const
{
    return m_isDigged;
}

// ===== Ladder =====
Ladder::Ladder()
{
    setTexture(ResourceManager::instance().getTexture(Textures::LADDER));
}

void Ladder::draw(sf::RenderWindow& window)
{
    if (m_sprite.has_value())
    {
        window.draw(*m_sprite);
    }
}

sf::FloatRect Ladder::getBounds()
{
    if (m_sprite.has_value())
    {
        return m_sprite->getGlobalBounds();
    }
    return sf::FloatRect({m_position.x, m_position.y}, {TILE_SIZE, TILE_SIZE});
}

// ===== Pole =====
Pole::Pole()
{
    setTexture(ResourceManager::instance().getTexture(Textures::POLE));
}

void Pole::draw(sf::RenderWindow& window)
{
    if (m_sprite.has_value())
    {
        window.draw(*m_sprite);
    }
}

sf::FloatRect Pole::getBounds()
{
    if (m_sprite.has_value())
    {
        return m_sprite->getGlobalBounds();
    }
    return sf::FloatRect({m_position.x, m_position.y}, {TILE_SIZE, TILE_SIZE});
}

// ===== Coin =====
Coin::Coin()
{
    setTexture(ResourceManager::instance().getTexture(Textures::COIN));
}

void Coin::draw(sf::RenderWindow& window)
{
    if (!m_collected && m_sprite.has_value())
    {
        window.draw(*m_sprite);
    }
}

sf::FloatRect Coin::getBounds()
{
    if (m_sprite.has_value())
    {
        return m_sprite->getGlobalBounds();
    }
    return sf::FloatRect({m_position.x, m_position.y}, {TILE_SIZE, TILE_SIZE});
}

void Coin::collect()
{
    m_collected = true;
}

bool Coin::isCollected() const
{
    return m_collected;
}
