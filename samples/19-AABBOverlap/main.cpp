#include <Timer.hpp>

#include <graphics/Image.hpp>
#include <graphics/Rasterizer.hpp>
#include <graphics/Window.hpp>

#include <random>
#include <vector>

using namespace sr;

constexpr int SCREEN_WIDTH  = 400;
constexpr int SCREEN_HEIGHT = 300;

std::vector<AABB> generateRandomAABBs( uint32_t n = 1 )
{
    std::vector<AABB> aabbs;
    // Create a few random aabbs
    std::minstd_rand rng { std::random_device()() };
    using param_type = std::uniform_int_distribution<>::param_type;
    std::uniform_int_distribution<> dist;
    for ( int i = 0; i < n; ++i )
    {
        int w    = dist( rng, param_type { 20, 50 } );
        int h    = dist( rng, param_type { 20, 50 } );
        int minX = dist( rng, param_type { 0, SCREEN_WIDTH - w } );
        int minY = dist( rng, param_type { 0, SCREEN_HEIGHT - h } );

        aabbs.emplace_back( glm::vec2 { minX, minY }, glm::vec2 { minX + w, minY + h } );
    }

    return aabbs;
}

int main()
{
    Window     window( "19-AABBOverlap", SCREEN_WIDTH, SCREEN_HEIGHT );
    Image      image { SCREEN_WIDTH, SCREEN_HEIGHT };
    Rasterizer rasterizer;
    Timer      timer;
    Text       fpsText { Font::DefaultFont, "FPS: 0" };
    glm::vec2  mousePos;

    // Setup the rasterizer's render target state.
    rasterizer.state.colorTarget = &image;

    // Setup some random AABBs
    std::vector<AABB> aabbs = generateRandomAABBs();
    // Create a dynamic AABB that will follow the mouse cursor.
    AABB dynamicAABB { { -20, -20 }, { 20, 20 } };

    bool freezeAABB = false;

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
                case SDLK_V:
                    window.toggleVSync();
                    break;
                case SDLK_R:  // Press R to generate random AABBs in the screen.
                    aabbs = generateRandomAABBs( aabbs.size() );
                    break;
                case SDLK_EQUALS:
                case SDLK_PLUS:
                    aabbs = generateRandomAABBs( aabbs.size() + 1 );
                    break;
                case SDLK_UNDERSCORE:
                case SDLK_MINUS:
                    if ( aabbs.size() > 0 )
                        aabbs = generateRandomAABBs( aabbs.size() - 1 );
                    break;
                case SDLK_SPACE:
                case SDLK_P:
                    freezeAABB = !freezeAABB;
                    break;
                case SDLK_RETURN:
                    if ( ( event.key.mod & SDL_KMOD_ALT ) != 0 )
                    {
                    case SDLK_F11:
                        window.toggleFullscreen();
                    }
                    break;
                }
                break;
            case SDL_EVENT_MOUSE_MOTION:
                if ( !freezeAABB )
                    mousePos = glm::round( window.clientToImage( event.motion.x, event.motion.y, image ) );
                break;
            }
        }

        // Translate the AABB to the mouse cursor.
        AABB translatedAABB = dynamicAABB + mousePos;

        // Check collisions.
        bool collides = false;
        for ( const auto& aabb: aabbs )
        {
            if ( auto mtv = translatedAABB.overlapXY( aabb ) )
            {
                translatedAABB += *mtv;
                collides = true;
            }
        }

        image.clear( Color::Black );

        // Draw the static AABB's
        rasterizer.state.color = Color::Blue;
        for ( const auto& aabb: aabbs )
        {
            rasterizer.drawAABB( aabb );
        }

        // Draw the dynamic AABB
        rasterizer.state.color = collides ? Color::Red : Color::Green;
        rasterizer.drawAABB( translatedAABB );

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