
#include <Timer.hpp>

#include <graphics/Image.hpp>
#include <graphics/Rasterizer.hpp>
#include <graphics/ResourceManager.hpp>
#include <graphics/TileMap.hpp>
#include <graphics/Window.hpp>

#include <LDtkLoader/Project.hpp>

using namespace sr;

/// <summary>
/// Load a spritesheet from an LDtk tileset.
/// </summary>
/// <param name="rootFolder">The root folder of the ldtk project.</param>
/// <param name="tileSet">The tileset to load.</param>
/// <returns>The spritesheet that contains the tiles of the tileset.</returns>
std::shared_ptr<SpriteSheet> fromTileSet( const std::filesystem::path& rootFolder, const ldtk::Tileset& tileSet )
{
    return std::make_shared<SpriteSheet>( rootFolder / tileSet.path, tileSet.tile_size, tileSet.tile_size, tileSet.padding, tileSet.spacing );
}

int main()
{
    Window     window( "Tile Maps", 800, 512 );
    Image      image { 800, 512 };
    Rasterizer rasterizer;
    Timer      timer;
    Text       fpsText( Font::DefaultFont, "FPS: 0" );

    // Setup the rasterizer's render target state.
    rasterizer.state.colorTarget = &image;

    // Load the ldtk project.
    ldtk::Project ldtkProject;
    ldtkProject.loadFromFile( "assets/levels/tilemap.ldtk" );
    auto& world      = ldtkProject.getWorld();
    auto& level      = world.getLevel( "Level_0" );
    auto& tilesLayer = level.getLayer( "Tiles" );
    auto& tileSet    = tilesLayer.getTileset();

    // Load the sprite sheet.
    auto        spriteSheet = fromTileSet( ldtkProject.getFilePath().directory(), tileSet );
    TileMap     tileMap( spriteSheet, tilesLayer.getGridSize().x, tilesLayer.getGridSize().y );
    Transform2D transform {
        { image.getWidth() / 2.0f, image.getHeight() / 2.0f }, 0.0f, { 1, 1 }, { tileMap.getWidth() / 2.0f, tileMap.getHeight() / 2.0f }
    };
    bool isPaused = false;

    // Load the tile ID's into the tilemap.
    for ( auto& tile: tilesLayer.allTiles() )
    {
        auto gridPos                  = tile.getGridPosition();
        tileMap[gridPos.x, gridPos.y] = tile.tileId;
    }

    while ( window )
    {
        timer.tick();

        SDL_Event event;
        while ( window.pollEvent( event ) )
        {
            switch ( event.type )
            {
            case SDL_EVENT_KEY_DOWN:
                switch ( event.key.key )
                {
                case SDLK_ESCAPE:
                    window.destroy();
                    break;
                case SDLK_P:
                case SDLK_SPACE:
                    isPaused = !isPaused;
                    break;
                case SDLK_R:
                    transform.setRotation( 0.0f );
                    transform.setScale( 1.0f );
                    break;
                case SDLK_LEFT:
                    transform.rotate( -0.01f );
                    break;
                case SDLK_RIGHT:
                    transform.rotate( 0.01f );
                    break;
                case SDLK_UP:
                    transform.scale( 1.01f );
                    std::cout << "Scale: " << transform.getScale().x << std::endl;
                    break;
                case SDLK_DOWN:
                    transform.scale( 0.99f );
                    std::cout << "Scale: " << transform.getScale().x << std::endl;
                    break;
                case SDLK_V:
                    window.toggleVSync();
                    break;
                case SDLK_RETURN:
                    if ( ( event.key.mod & SDL_KMOD_ALT ) != 0 )
                    {
                    case SDLK_F11:
                        window.toggleFullscreen();
                    }
                    break;
                case SDLK_S:
                    if ( ( event.key.mod & SDL_KMOD_CTRL ) != 0 )
                    {
                        image.save( "screenshot.bmp" );
                        std::cout << "Screenshot saved..." << std::endl;
                    }
                    break;
                }
                break;
            case SDL_EVENT_MOUSE_WHEEL:
                transform.scale( event.wheel.integer_y > 0 ? 1.01f : 0.99f );
                std::cout << "Scale: " << transform.getScale().x << std::endl;
                break;
            }
        }

        image.clear( Color::Black );

        if ( !isPaused )
        {
            transform.rotate( static_cast<float>( timer.elapsedSeconds() ) );
        }

        rasterizer.drawTileMap( tileMap, transform );

                if ( timer.totalSeconds() > 1.0 )
        {
            fpsText = std::format( "FPS: {:.0f}", timer.FPS() );
            timer.reset();
        }

        rasterizer.drawText( fpsText, 10, 10 );


        window.clear( Color::Black );
        window.present( image );
    }

    return 0;
}