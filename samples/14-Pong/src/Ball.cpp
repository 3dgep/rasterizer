#include <Ball.hpp>

#include <graphics/Color.hpp>

using namespace sr::graphics;

Ball::Ball()
: m_AABB { { -2, -2, 0 }, { 2, 2, 0 } }
{}

Ball::Ball( const glm::vec2& position, const glm::vec2& velocity )
: Ball {}
{
    m_Position = position;
    m_Velocity = velocity;
}

void Ball::update( float deltaTime )
{
    m_Position += m_Velocity * deltaTime;
}

void Ball::draw( sr::Rasterizer& rasterizer )
{
    rasterizer.state.color    = Color::White;
    rasterizer.state.fillMode = FillMode::Solid;

    rasterizer.drawAABB( getAABB() );

#if _DEBUG
    rasterizer.state.color = Color::Red;
    rasterizer.state.fillMode = FillMode::WireFrame;
    rasterizer.drawAABB( getAABB() );
#endif
}

int Ball::getRadius() const noexcept
{
    return 2;  // static_cast<int>( m_AABB.width() / 2.0f );
}

sr::AABB Ball::getAABB() const noexcept
{
    return m_AABB + glm::vec3 { m_Position, 0 };
}

const glm::vec2& Ball::getPosition() const noexcept
{
    return m_Position;
}

void Ball::setPosition( const glm::vec2& position )
{
    m_Position = position;
}

void Ball::setX( float x )
{
    m_Position.x = x;
}

const glm::vec2& Ball::getVelocity() const noexcept
{
    return m_Velocity;
}

void Ball::setVelocity( const glm::vec2& velocity )
{
    m_Velocity = velocity;
}