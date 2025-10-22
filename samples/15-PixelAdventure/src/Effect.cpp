#include <Effect.hpp>

using namespace sr;

Effect::Effect( const SpriteAnimation& spriteAnim, const Transform2D& transform )
: spriteAnim{&spriteAnim}
, transform{transform}
{}

void Effect::update( float deltaTime )
{
    time += deltaTime;
}

void Effect::draw( Rasterizer& rasterizer ) const
{
    if ( spriteAnim )
        rasterizer.drawSprite( spriteAnim->at( time ), transform );
}

bool Effect::isDone() const noexcept
{
    if ( spriteAnim )
        return time > spriteAnim->getDuration();

    // Effects without a valid sprite animation are always done.
    return true;
}
