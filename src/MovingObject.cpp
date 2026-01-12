#include "MovingObject.h"

void MovingObject::setSpeed(float speed)
{
    m_speed = speed;
}

void MovingObject::applyGravity(float deltaTime)
{
    const float GRAVITY = 500.0f;
    m_velocity.y += GRAVITY * deltaTime;
}


