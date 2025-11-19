#include <PowerUp.hpp>

using namespace sr;

PowerUp::PowerUp( std::shared_ptr<SpriteSheet> spriteSheet, std::span<const int> frames, Type type )
: sprites { std::move( spriteSheet ), FPS, frames }
, aabb { { 0, 0, 0 }, { 16, 8, 0 } }
, type { type }
{}

void PowerUp::update( float deltaTime )
{
    if ( type == None )
        return;

    sprites.update( deltaTime );

    glm::vec2 pos = transform.getPosition();
    pos += glm::vec2 { 0, SPEED } * deltaTime;
    transform.setPosition( pos );
}

void PowerUp::draw( Rasterizer& rasterizer ) const
{
    if ( type == None )
        return;

    const int x = static_cast<int>( transform.getPosition().x );
    const int y = static_cast<int>( transform.getPosition().y );

    auto r = rasterizer;
    r.state.color = Color::Black;
    r.drawSprite( sprites, x + 2, y + 2);
    r.state.color = Color::White;
    r.drawSprite( sprites, x, y );

#ifndef NDEBUG
    r.state.color = Color::Yellow;
    r.state.fillMode = FillMode::WireFrame;
    r.drawAABB( getAABB() );
#endif
}

void PowerUp::setPosition( const glm::vec2& pos ) noexcept
{
    transform.setPosition( pos );
}

const glm::vec2& PowerUp::getPosition() const noexcept
{
    return transform.getPosition();
}

AABB PowerUp::getAABB() const noexcept
{
    return transform * aabb;
}

bool PowerUp::checkCollision( const AABB& _aabb ) const
{
    return getAABB().intersect( _aabb );
}
