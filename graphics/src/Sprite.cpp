#include <graphics/ResourceManager.hpp>
#include <graphics/Sprite.hpp>

using namespace sr::graphics;

Sprite::Sprite( const std::filesystem::path& fileName, const BlendMode& blendMode )
: m_Image { ResourceManager::loadImage( fileName ) }
, m_BlendMode { blendMode }
{
    if ( m_Image )
        m_Rect = RectI { 0, 0, m_Image->getWidth(), m_Image->getHeight() };
}

Sprite::Sprite( const std::filesystem::path& fileName, const math::RectI& rect, const BlendMode& blendMode )
: m_Image { ResourceManager::loadImage( fileName ) }
, m_Rect { rect }
, m_BlendMode { blendMode }
{}