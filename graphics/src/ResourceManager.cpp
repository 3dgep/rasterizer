#include <graphics/ResourceManager.hpp>
#include <hash.hpp>

#include <unordered_map>

using namespace sr::graphics;

struct FontKey
{
    std::filesystem::path fontFile;
    float                 size;

    bool operator==( const FontKey& other ) const
    {
        return fontFile == other.fontFile && size == other.size;
    }
};

// Hasher for a FontKey.
template<>
struct std::hash<FontKey>
{
    size_t operator()( const FontKey& key ) const noexcept
    {
        std::size_t seed = 0;

        hash_combine( seed, key.fontFile );
        hash_combine( seed, key.size );

        return seed;
    }
};

namespace
{
// Image store.
using ImageMap = std::unordered_map<std::filesystem::path, std::shared_ptr<Image>>;
ImageMap& im()
{
    static ImageMap map;
    return map;
}

// Font store.
using FontMap = std::unordered_map<FontKey, std::shared_ptr<Font>>;

// Font map
FontMap& fm()
{
    static FontMap map;
    return map;
}
}  // namespace

std::shared_ptr<Image> ResourceManager::loadImage( const std::filesystem::path& filePath )
{
    auto& imageMap = im();

    const auto iter = imageMap.find( filePath );
    if ( iter == imageMap.end() )
    {
        auto image = std::make_shared<Image>( filePath );
        imageMap.insert( { filePath, image } );
        return image;
    }

    return iter->second;
}

void ResourceManager::clearImages()
{
    im().clear();
}

std::shared_ptr<SpriteSheet> ResourceManager::loadSpriteSheet( const std::filesystem::path& filePath, std::optional<int> spriteWidth, std::optional<int> spriteHeight, int padding, int margin, const BlendMode& blendMode )
{
    auto image = loadImage( filePath );
    return std::make_shared<SpriteSheet>( image, spriteWidth, spriteHeight, padding, margin, blendMode );
}

std::shared_ptr<SpriteSheet> ResourceManager::loadSpriteSheet( const std::filesystem::path& filePath, std::span<const math::RectI> rects, const BlendMode& blendMode )
{
    auto image = loadImage( filePath );
    return std::make_shared<SpriteSheet>( image, rects, blendMode );
}

std::shared_ptr<Font> ResourceManager::loadFont( const std::filesystem::path& filePath, float size )
{
    FontKey    key { filePath, size };
    auto&      fontMap = fm();
    const auto iter    = fontMap.find( key );

    if ( iter == fontMap.end() )
    {
        auto font    = std::make_shared<Font>( filePath, size );
        fontMap[key] = font;

        return font;
    }

    return iter->second;
}

std::shared_ptr<Font> ResourceManager::loadFont( float size )
{
    FontKey    key { "__default__", size };
    auto&      fontMap = fm();
    const auto iter    = fontMap.find( key );

    if ( iter == fontMap.end() )
    {
        auto font    = std::make_shared<Font>( size );
        fontMap[key] = font;

        return font;
    }

    return iter->second;
}

void ResourceManager::clearFonts()
{
    fm().clear();
}

void ResourceManager::clear()
{
    clearImages();
    clearFonts();
}