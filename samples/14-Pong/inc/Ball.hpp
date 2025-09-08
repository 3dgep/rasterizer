#pragma once

#include <math/AABB.hpp>

#include <graphics/Rasterizer.hpp>

class Ball
{
public:
    Ball();
    Ball( const glm::vec2& position, const glm::vec2& velocity );

    void update( float deltaTime );

    void draw( sr::Rasterizer& rasterizer );

    sr::AABB getAABB() const noexcept;

    const glm::vec2& getPosition() const noexcept;
    void             setPosition( const glm::vec2& position );

    const glm::vec2& getVelocity() const noexcept;
    void             setVelocity( const glm::vec2& velocity );

private:
    sr::AABB m_AABB;
    glm::vec2 m_Position;
    glm::vec2 m_Velocity;

};