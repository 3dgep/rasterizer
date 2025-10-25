#include <Ball.hpp>

#include <graphics/ResourceManager.hpp>

using namespace sr;

inline Circle operator*( const Camera2D& camera, const Circle& c )
{
    return { camera * c.center, camera.getZoom() * c.radius };
}

Ball::Ball()
: circle { { 0, 0 }, radius }
, sprite { ResourceManager::loadImage( "assets/Arkanoid/vaus.png" ), RectI { 0, 40, 16, 8 }, BlendMode::AlphaDiscard }
{
    transform.setAnchor( { radius, radius } );
}

Ball::Ball( const glm::vec2& position )
: transform { position }
, circle { position, radius }
, sprite { ResourceManager::loadImage( "assets/Arkanoid/vaus.png" ), RectI { 0, 40, 16, 8 }, BlendMode::AlphaDiscard }
{
    transform.setAnchor( { radius, radius } );
}

void Ball::update( float deltaTime )
{
    auto position = transform.getPosition();

    position += velocity * deltaTime;
    circle.center = position;

    transform.setPosition( position );
}

void Ball::draw( Rasterizer& rasterizer )
{
    const int x = static_cast<int>( std::round( transform.getPosition().x - circle.radius ) );
    const int y = static_cast<int>( std::round( transform.getPosition().y - circle.radius ) );

    rasterizer.drawSprite( sprite, x, y );

#if _DEBUG
    {
        auto r           = rasterizer;
        r.state.color    = Color::Yellow;
        r.state.fillMode = FillMode::WireFrame;
        r.drawCircle( circle );
    }
#endif
}

void Ball::setPosition( const glm::vec2& pos )
{
    transform.setPosition( pos );
}

const glm::vec2& Ball::getPosition() const
{
    return transform.getPosition();
}

void Ball::setVelocity( const glm::vec2& vel )
{
    velocity = vel;
}

const glm::vec2& Ball::getVelocity() const
{
    return velocity;
}

void Ball::setCircle( const Circle& _circle )
{
    circle = _circle;
    transform.setPosition( circle.center );
}

const Circle& Ball::getCircle() const
{
    return circle;
}
