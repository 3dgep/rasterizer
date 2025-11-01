#include "Ball.hpp"

#include <cmath>

Ball::Ball( const glm::vec2& pos, float radius, float mass )
: circle { pos, radius }
, mass { mass }
{}

const glm::vec2& Ball::getPosition() const
{
    return circle.center;
}

void Ball::setPosition( const glm::vec2& pos )
{
    circle.center = pos;
}

const glm::vec2& Ball::getVelocity() const
{
    return velocity;
}

void Ball::setVelocity( const glm::vec2& vel )
{
    velocity = vel;
}

const glm::vec2& Ball::getAcceleration() const
{
    return acceleration;
}

void Ball::setAcceleration( const glm::vec2& acc )
{
    acceleration = acc;
}

void Ball::addAcceleration( const glm::vec2& acc )
{
    acceleration += acc;
}

void Ball::addForce( const glm::vec2& force )
{
    acceleration += force * ( 1.0f / mass );
}

float Ball::getRadius() const
{
    return circle.radius;
}

void Ball::setRadius( float radius )
{
    circle.radius = radius;
}

float Ball::getMass() const
{
    return mass;
}

void Ball::setMass( float _mass )
{
    mass = _mass;
}

std::optional<HitInfo> Ball::checkCollision( const Ball& other ) const
{
    glm::vec2   d = other.circle.center - circle.center;  // Displacement.
    const float l = glm::length2( d );
    const float r = other.circle.radius + circle.radius;  // Sum of the radii.

    if ( l < ( r * r ) )
    {
        HitInfo hit;

        hit.normal = glm::normalize( d );
        hit.depth  = r - std::sqrt( l );

        return hit;
    }

    return {};
}

void Ball::update( float deltaTime )
{
    velocity += acceleration * deltaTime;
    circle.center += velocity * deltaTime;
}

void Ball::draw( const sr::graphics::Rasterizer& rasterizer ) const
{
    auto r        = rasterizer;

    r.state.color = sr::graphics::Color::Red;
    r.state.fillMode = sr::graphics::FillMode::WireFrame;

    r.drawCircle( circle );
}
