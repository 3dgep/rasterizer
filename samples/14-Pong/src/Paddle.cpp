#include <Paddle.hpp>

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

sr::math::AABB Paddle::getAABB() const noexcept
{
    return m_AABB + glm::vec3 { m_Position, 0 };
}

void Paddle::draw( sr::Rasterizer& rasterizer )
{
    rasterizer.state.color    = Color::White;
    rasterizer.state.fillMode = FillMode::Solid;

    rasterizer.drawAABB( getAABB() );

#if _DEBUG
    rasterizer.state.color    = Color::Red;
    rasterizer.state.fillMode = FillMode::WireFrame;
    rasterizer.drawAABB( getAABB() );
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
            if ( ballAABB.min.x < paddleAABB.min.x )
            {
                // Ball is to the left of the paddle.
                p.x -= xOverlap;  // Resolve the overlap.
            }
            else
            {
                p.x += xOverlap;
            }

            v.x *= -1.0f;  // Reverse the ball's velocity.
        }
        else
        {
            if ( ballAABB.min.y < paddleAABB.min.y )
            {
                // Ball is above the paddle.
                p.y -= yOverlap;
            }
            else
            {
                p.y += yOverlap;
            }

            v.y *= -1.0f;
        }

        ball.setPosition( p );
        ball.setVelocity( v );

        return true;
    }

    return false;
}