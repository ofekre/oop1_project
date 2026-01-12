#pragma once
#include "GameObject.h"

class Wall : public GameObject
{
public:
    Wall();
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() override;
    void processCollision(Collider& other) override { other.handleCollision(*this); }
};

// Abstract base class for all floor types
class BaseFloor : public GameObject
{
public:
    virtual ~BaseFloor() = default;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() override;
    
protected:
    BaseFloor() = default;  // Can't instantiate directly
};

class Floor : public BaseFloor
{
public:
    Floor();
    void processCollision(Collider& other) override { other.handleCollision(*this); }
};

class DiggableFloor : public BaseFloor
{
public:
    DiggableFloor();
    void dig();
    void restore();
    bool isDigged() const;
    void processCollision(Collider& other) override { other.handleCollision(*this); }

private:
    bool m_isDigged = false;
};

class Ladder : public GameObject
{
public:
    Ladder();
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() override;
    void processCollision(Collider& other) override { other.handleCollision(*this); }
};

class Pole : public GameObject
{
public:
    Pole();
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() override;
    void processCollision(Collider& other) override { other.handleCollision(*this); }
};

class Coin : public GameObject
{
public:
    Coin();
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() override;
    void collect();
    bool isCollected() const;
    void processCollision(Collider& other) override { other.handleCollision(*this); }

private:
    bool m_collected = false;
};