#include <graphics/ResourceManager.hpp>

#include <unordered_map>

using namespace sr::graphics;

namespace
{
// Image store.
std::unordered_map<std::filesystem::path, std::shared_ptr<Image>> g_ImageMap;

}  // namespace

std::shared_ptr<Image> ResourceManager::loadImage( const std::filesystem::path& filePath )
{
    const auto iter = g_ImageMap.find( filePath );

    if ( iter == g_ImageMap.end() )
    {
        auto image = std::make_shared<Image>( filePath );

        g_ImageMap.insert( { filePath, image } );

        return image;
    }

    return iter->second;
}

std::shared_ptr<SpriteSheet> ResourceManager::loadSpriteSheet( const std::filesystem::path& filePath, uint32_t columns, uint32_t rows, uint32_t padding, uint32_t margin, const BlendMode& blendMode )
{
    auto image = loadImage( filePath );
    return std::make_shared<SpriteSheet>( image, rows, columns, padding, margin, blendMode );
}