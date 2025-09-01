#include <graphics/SpriteAnimation.hpp>

#include <numeric>  // for std::iota.

using namespace sr::graphics;

SpriteAnimation::SpriteAnimation( std::shared_ptr<SpriteSheet> spriteSheet, float fps, std::span<const int> frames )
: m_SpriteSheet { std::move( spriteSheet ) }
, m_FrameRate { fps }
, m_Time { 0.0f }
{
    if ( frames.empty() )
    {
        if ( m_SpriteSheet )
        {
            m_Frames.resize( m_SpriteSheet->getNumSprites() );
            std::ranges::iota( m_Frames, 0 );
        }
    }
    else
    {
        m_Frames = std::vector( frames.begin(), frames.end() );  // C++23 introduces the range constructor.
    }
}

void SpriteAnimation::update( float deltaTime ) noexcept
{
    m_Time += deltaTime;
}

float SpriteAnimation::getDuration() const noexcept
{
    return static_cast<float>( m_Frames.size() ) / m_FrameRate;
}

bool SpriteAnimation::isDone() const noexcept
{
    return m_Time > getDuration();
}

void SpriteAnimation::reset() noexcept
{
    m_Time = 0.0f;
}

const Sprite& SpriteAnimation::operator[]( size_t frame ) const noexcept
{
    if ( m_SpriteSheet )
    {
        const size_t i = m_Frames[frame % m_Frames.size()];
        return m_SpriteSheet->getSprite( i );
    }

    static const Sprite emptySprite;
    return emptySprite;
}

const Sprite& SpriteAnimation::at( float time ) const noexcept
{
    size_t frame = static_cast<size_t>( time * m_FrameRate );
    return operator[]( frame );
}

SpriteAnimation::operator const Sprite&() const noexcept
{
    return at( m_Time );
}