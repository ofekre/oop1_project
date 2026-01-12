#include "Enemy.h"
#include "Resourcemanager.h"
#include "Constants.h"

using namespace GameConstants;

Enemy::Enemy()
{
    setTexture(ResourceManager::instance().getTexture(Textures::ENEMY));
    m_speed = ENEMY_SPEED;
    m_velocity = {0.f, 0.f};
}

void Enemy::draw(sf::RenderWindow& window)
{
    if (m_sprite.has_value())
    {
        window.draw(*m_sprite);
    }
}

sf::FloatRect Enemy::getBounds()
{
    if (m_sprite.has_value())
    {
        return m_sprite->getGlobalBounds();
    }
    return sf::FloatRect({m_position.x, m_position.y}, {TILE_SIZE, TILE_SIZE});
}

void Enemy::move(float deltaTime)
{
    m_position.x += m_velocity.x * deltaTime;
    m_position.y += m_velocity.y * deltaTime;
    
    if (m_sprite.has_value())
    {
        m_sprite->setPosition(m_position);
    }
}

void Enemy::updateAI()
{
    // TODO: AI logic
}

void Enemy::resetToStart()
{
    m_position = m_startPosition;
    m_velocity = {0.f, 0.f};
    
    if (m_sprite.has_value())
    {
        m_sprite->setPosition(m_position);
    }
}
