#pragma once

#include "Ball.hpp"

#include <graphics/Rasterizer.hpp>
#include <math/AABB.hpp>

#include <glm/vec2.hpp>

class Paddle
{
public:
    Paddle();
    Paddle( const glm::vec2& position );

    const glm::vec2& getPosition() const noexcept;
    void             setPosition( const glm::vec2& position );

    void setY( float y );
    float getY() const noexcept;

    const glm::vec2& getVelocity() const noexcept;
    void             setVelocity( const glm::vec2& velocity );

    sr::math::AABB   getAABB() const noexcept;

    void draw( sr::Rasterizer& rasterizer );

    /// <summary>
    /// Check the AABB of the ball with the AABB of this collider and update
    /// the ball's position and velocity to resolve the collision.
    /// </summary>
    /// <param name="ball">The ball to check for collisions with.</param>
    /// <returns>`true` if a collision occured, `false` otherwise.</returns>
    bool checkCollision( Ball& ball );

private:
    sr::math::AABB m_AABB;

    glm::vec2 m_Position { 0 };
    glm::vec2 m_Velocity { 0 };
};