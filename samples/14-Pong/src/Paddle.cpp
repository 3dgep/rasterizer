#include <Paddle.hpp>

#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>

#include <algorithm>
#include <cmath>

using namespace sr::graphics;

Paddle::Paddle()
: m_AABB { { -2, -10, 0 }, { 2, 10, 0 } }
{}

Paddle::Paddle( const glm::vec2& position )
: Paddle {}
{
    m_Position = position;
}

const glm::vec2& Paddle::getPosition() const noexcept
{
    return m_Position;
}

void Paddle::setPosition( const glm::vec2& position )
{
    m_Position = position;
}

void Paddle::setY( float y )
{
    m_Position.y = y;
}

float Paddle::getY() const noexcept
{
    return m_Position.y;
}

const glm::vec2& Paddle::getVelocity() const noexcept
{
    return m_Velocity;
}

void Paddle::setVelocity( const glm::vec2& velocity )
{
    m_Velocity = velocity;
}

sr::math::AABB Paddle::getAABB() const noexcept
{
    return m_AABB + glm::vec3 { m_Position, 0 };
}

void Paddle::draw( sr::Rasterizer& rasterizer )
{
    rasterizer.state.color    = Color::White;
    rasterizer.state.fillMode = FillMode::Solid;

    rasterizer.drawAABB( getAABB() );

#ifndef NDEBUG
    rasterizer.state.color    = Color::Red;
    rasterizer.state.fillMode = FillMode::WireFrame;
    rasterizer.drawAABB( getAABB() );

    rasterizer.state.color = Color::Lime;
    rasterizer.drawLine( m_Position, m_Position + m_Velocity );
#endif
}

bool Paddle::checkCollision( Ball& ball )
{
    sr::AABB  paddleAABB = getAABB();
    sr::AABB  ballAABB   = ball.getAABB();
    glm::vec2 p          = ball.getPosition();
    glm::vec2 v          = ball.getVelocity();

    // First check if any collision occurs.
    if ( paddleAABB.intersect( ball.getAABB() ) )
    {
        // Source: https:://grok.com (March 6, 2025). "What are the basic steps to implementing 2D collision detection and response with AABBs?"

        // Compute overlap
        float xOverlap = std::min( paddleAABB.max.x, ballAABB.max.x ) - std::max( paddleAABB.min.x, ballAABB.min.x );
        float yOverlap = std::min( paddleAABB.max.y, ballAABB.max.y ) - std::max( paddleAABB.min.y, ballAABB.min.y );

        if ( xOverlap < yOverlap )
        {
            // The "minimum translation vector" (MTV) is in the x-axis.
            // This means the ball hit the front or back face of the paddle.
            if ( ballAABB.min.x < paddleAABB.min.x )
            {
                // Ball is to the left of the paddle.
                p.x -= xOverlap;  // Resolve the overlap.
            }
            else
            {
                p.x += xOverlap;
            }

            // Calculate where on the paddle the ball hit
            // hitOffset ranges from -1 (top) to +1 (bottom), with 0 being the center
            float paddleCenter = paddleAABB.center().y;
            float ballCenter   = p.y;
            float hitOffset    = ( ballCenter - paddleCenter ) / ( paddleAABB.height() / 2.0f );
            hitOffset          = std::clamp( hitOffset, -1.0f, 1.0f );

            // Reverse the ball's X velocity
            v.x *= -1.0f;

            // Adjust the ball's Y velocity based on where it hit the paddle
            // This creates a bounce angle: hitting the top makes it go up, bottom makes it go down
            float           currentSpeed   = glm::length( v );
            constexpr float maxBounceAngle = glm::radians( 60.0f );  // Maximum bounce angle from horizontal
            float           bounceAngle    = hitOffset * maxBounceAngle;

            // Calculate new velocity components
            // Preserve the speed but change the direction
            v.x = std::copysign( currentSpeed * std::cos( bounceAngle ), v.x );
            v.y = currentSpeed * std::sin( bounceAngle );

            // Add some influence from the paddle's velocity for more dynamic gameplay
            v.y += m_Velocity.y * 0.2f;
        }
        else
        {
            // The "minimum translation vector" (MTV) is in the y-axis.
            // This means the ball hit the top or bottom of the paddle.

            // Resolve the overlap
            if ( ballAABB.min.y < paddleAABB.min.y )
            {
                // Ball is above the paddle (hit the top).
                p.y -= yOverlap;
                // Make the ball bounce upward
                v.y = -std::abs( v.y );
            }
            else
            {
                // Ball is below the paddle (hit the bottom).
                p.y += yOverlap;
                // Make the ball bounce downward
                v.y = std::abs( v.y );
            }

            // Add some influence from the paddle's velocity
            v.y += m_Velocity.y * 0.2f;
        }

        ball.setPosition( p );
        ball.setVelocity( v );

        return true;
    }

    return false;
}