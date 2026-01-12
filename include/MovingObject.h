#pragma once
#include "GameObject.h"

class MovingObject : public GameObject
{
public:
    virtual void move(float deltaTime) = 0;
    void setSpeed(float speed);
    void applyGravity(float deltaTime);

protected:
    float m_speed = 0.f;
    sf::Vector2f m_velocity = {0.f, 0.f};
};