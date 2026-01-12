#include "Player.h"
#include "Resourcemanager.h"
#include "Constants.h"
#include "StaticObject.h"
#include "Enemy.h"

using namespace GameConstants;

Player::Player()
{
    setTexture(ResourceManager::instance().getTexture(Textures::PLAYER));
    m_speed = PLAYER_SPEED;
    m_velocity = {0.f, 0.f};
}

void Player::draw(sf::RenderWindow& window)
{
    if (m_sprite.has_value())
    {
        window.draw(*m_sprite);
    }
    else
    {
        // Debug fallback - draw red rectangle if no sprite
        sf::RectangleShape rect({TILE_SIZE, TILE_SIZE});
        rect.setPosition(m_position);
        rect.setFillColor(sf::Color::Red);
        window.draw(rect);
    }
}

sf::FloatRect Player::getBounds()
{
    // Make collision box slightly smaller than visual size to prevent getting stuck
    const float SHRINK = 4.0f;  // Shrink by 4 pixels on each side
    
    if (m_sprite.has_value())
    {
        sf::FloatRect bounds = m_sprite->getGlobalBounds();
        return sf::FloatRect(
            {bounds.position.x + SHRINK, bounds.position.y + SHRINK},
            {bounds.size.x - SHRINK * 2, bounds.size.y - SHRINK * 2}
        );
    }
    return sf::FloatRect(
        {m_position.x + SHRINK, m_position.y + SHRINK},
        {TILE_SIZE - SHRINK * 2, TILE_SIZE - SHRINK * 2}
    );
}

void Player::move(float deltaTime)
{
    // Apply gravity if not on ground, ladder, or pole
    if (!m_onGround && !m_onLadder && !m_onPole)
    {
        m_velocity.y += GameConstants::GRAVITY * deltaTime;
    }
    else
    {
        // On ground/ladder/pole - no vertical velocity from gravity
        if (m_onGround)
        {
            m_velocity.y = 0;
        }
    }
    
    m_position.x += m_velocity.x * deltaTime;
    m_position.y += m_velocity.y * deltaTime;
    
    // Reset state flags - will be set again by collision checks
    m_onGround = false;
    m_onLadder = false;
    m_onPole = false;
    
    if (m_sprite.has_value())
    {
        m_sprite->setPosition(m_position);
    }
}

void Player::handleInput()
{
    m_velocity.x = 0;
    m_velocity.y = 0;

    // Left/Right movement always allowed
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        m_velocity.x = -m_speed;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        m_velocity.x = m_speed;
    }

    // UP movement only on ladder/pole
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
    {
        if (m_onLadder)
        {
            m_velocity.y = -m_speed;  // Can climb only on ladder/pole
        }
    }
    // DOWN movement always allowed (falling/descending)
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
    {
        if (m_onLadder)
        {
            m_velocity.y = m_speed; // Can climb only on ladder/pole
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
    {
        dig(true);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X))
    {
        dig(false);
    }
}

void Player::dig(bool left)
{
    // TODO: Implement digging
}

// ===== Collision Handlers =====

void Player::handleCollision(Wall& wall)
{
    // Stop player from entering wall - push back
    sf::FloatRect playerBounds = getBounds();
    sf::FloatRect wallBounds = wall.getBounds();
    
    // Calculate overlap
    float overlapLeft = (playerBounds.position.x + playerBounds.size.x) - wallBounds.position.x;
    float overlapRight = (wallBounds.position.x + wallBounds.size.x) - playerBounds.position.x;
    float overlapTop = (playerBounds.position.y + playerBounds.size.y) - wallBounds.position.y;
    float overlapBottom = (wallBounds.position.y + wallBounds.size.y) - playerBounds.position.y;
    
    // Find smallest overlap
    float minOverlapX = (overlapLeft < overlapRight) ? -overlapLeft : overlapRight;
    float minOverlapY = (overlapTop < overlapBottom) ? -overlapTop : overlapBottom;
    
    // Push in direction of smallest overlap
    if (std::abs(minOverlapX) < std::abs(minOverlapY))
    {
        m_position.x += minOverlapX;
    }
    else
    {
        m_position.y += minOverlapY;
        if (minOverlapY < 0)
        {
            m_onGround = true;
            m_velocity.y = 0;  // Stop falling when landing
        }
    }
    
    if (m_sprite.has_value())
    {
        m_sprite->setPosition(m_position);
    }
}

void Player::handleCollision(Floor& floor)
{
    // Same collision logic as wall - solid surface
    sf::FloatRect playerBounds = getBounds();
    sf::FloatRect floorBounds = floor.getBounds();
    
    float overlapLeft = (playerBounds.position.x + playerBounds.size.x) - floorBounds.position.x;
    float overlapRight = (floorBounds.position.x + floorBounds.size.x) - playerBounds.position.x;
    float overlapTop = (playerBounds.position.y + playerBounds.size.y) - floorBounds.position.y;
    float overlapBottom = (floorBounds.position.y + floorBounds.size.y) - playerBounds.position.y;
    
    float minOverlapX = (overlapLeft < overlapRight) ? -overlapLeft : overlapRight;
    float minOverlapY = (overlapTop < overlapBottom) ? -overlapTop : overlapBottom;
    
    if (std::abs(minOverlapX) < std::abs(minOverlapY))
    {
        m_position.x += minOverlapX;
    }
    else
    {
        m_position.y += minOverlapY;
        if (minOverlapY < 0)
        {
            m_onGround = true;
            m_velocity.y = 0;  // Stop falling when landing
        }
    }
    
    if (m_sprite.has_value())
    {
        m_sprite->setPosition(m_position);
    }
}

void Player::handleCollision(DiggableFloor& floor)
{
    if (!floor.isDigged())
    {
        // Treat as solid - block from all directions like a wall
        sf::FloatRect playerBounds = getBounds();
        sf::FloatRect floorBounds = floor.getBounds();
        
        float overlapLeft = (playerBounds.position.x + playerBounds.size.x) - floorBounds.position.x;
        float overlapRight = (floorBounds.position.x + floorBounds.size.x) - playerBounds.position.x;
        float overlapTop = (playerBounds.position.y + playerBounds.size.y) - floorBounds.position.y;
        float overlapBottom = (floorBounds.position.y + floorBounds.size.y) - playerBounds.position.y;
        
        float minOverlapX = (overlapLeft < overlapRight) ? -overlapLeft : overlapRight;
        float minOverlapY = (overlapTop < overlapBottom) ? -overlapTop : overlapBottom;
        
        if (std::abs(minOverlapX) < std::abs(minOverlapY))
        {
            m_position.x += minOverlapX;
        }
        else
        {
            m_position.y += minOverlapY;
            if (minOverlapY < 0)
            {
                m_onGround = true;
                m_velocity.y = 0;  // Stop falling when landing
            }
        }
        
        if (m_sprite.has_value())
        {
            m_sprite->setPosition(m_position);
        }
    }
    // If digged, player falls through
}

void Player::handleCollision(Ladder& ladder)
{
    m_onLadder = true;
    m_velocity.y = 0;  // Stop falling - grab ladder
    // Player stays at current position - no snapping
}

void Player::handleCollision(Pole& pole)
{
    m_onPole = true;
    m_velocity.y = 0;
    
    // Snap to pole position - player hangs ON the pole
    sf::FloatRect poleBounds = pole.getBounds();
    m_position.y = poleBounds.position.y;
    
    if (m_sprite.has_value())
    {
        m_sprite->setPosition(m_position);
    }
}

void Player::handleCollision(Coin& coin)
{
    if (!coin.isCollected())    
    {
        coin.collect();
        addScore(COIN_SCORE_MULTIPLIER);  // Add score
    }
}

void Player::handleCollision(Enemy& enemy)
{
    // Lose life only if not invincible
    if (!isInvincible())
    {
        loseLife();
    }
    
    // Always reset position when hitting enemy (even during invincibility)
    m_needsReset = true;
}

void Player::update(float deltaTime)
{
    // Count down invincibility timer
    if (m_invincibilityTimer > 0)
    {
        m_invincibilityTimer -= deltaTime;
    }
}

void Player::loseLife()
{
    if (m_lives > 0 && !isInvincible())
    {
        --m_lives;
        m_invincibilityTimer = 2.0f;  // 2 seconds of invincibility
        m_needsReset = true;  // Signal board to reset positions
    }
}

void Player::resetToStart()
{
    m_position = m_startPosition;
    m_velocity = {0.f, 0.f};
    m_onGround = true;
    m_onLadder = false;
    m_onPole = false;
    m_needsReset = false;
    
    if (m_sprite.has_value())
    {
        m_sprite->setPosition(m_position);
    }
}
