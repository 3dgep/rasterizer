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
        int w = dist( rng, param_type { 20, 50 } );
        int h = dist( rng, param_type { 20, 50 } );
        int x = dist( rng, param_type { 0, SCREEN_WIDTH - w } );
        int y = dist( rng, param_type { 0, SCREEN_HEIGHT - h } );

        aabbs.emplace_back( glm::vec2 { x - w, y - h }, glm::vec2 { x + w, y + h } );
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
    AABB   aabb { { -20, -20 }, { 20, 20 } };
    Circle circle { { 0.0f, 0.0f }, 20.0f };

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
        AABB   translatedAABB   = aabb + mousePos;
        Circle translatedCircle = circle + mousePos;

        // Expand the AABB a bit to make the collision "pixel perfect".
        AABB   collisionAABB { translatedAABB.min - glm::vec3 { 1.0f }, translatedAABB.max + glm::vec3 { 1.0f } };
        Circle collisionCircle { translatedCircle.center, translatedCircle.radius + 1.0f };

        // Check collisions.
        bool aabbCollides   = false;
        bool circleCollides = false;

        for ( const auto& c: aabbs )
        {
            if ( auto mtv = c.overlapXY( collisionAABB ) )
            {
                collisionAABB += *mtv;
                aabbCollides = true;
            }
            if ( auto mtv = c.overlap( collisionCircle ) )
            {
                collisionCircle.center += *mtv;
                circleCollides = true;
            }
        }

        image.clear( Color::Black );

        {
            auto r = rasterizer;

            // Draw the static AABB's
            for ( const auto& aabb: aabbs )
            {
                r.state.fillMode = FillMode::Solid;
                r.state.color    = Color::Blue;
                r.drawAABB( aabb );

                r.state.fillMode = FillMode::WireFrame;
                r.state.color    = Color::Orange;
                r.drawAABB( aabb );
            }

            // Draw the translated AABB either red (colliding) or green (not colliding)
            r.state.fillMode = FillMode::Solid;
            r.state.color    = aabbCollides ? Color::Red : Color::Green;
            r.drawAABB( collisionAABB );

            // Draw the translated Circle either magenta (colliding) or green (not colliding)
            r.state.color = circleCollides ? Color::Magenta : Color::Green;
            r.drawCircle( collisionCircle );

            // Draw the collision AABB as a yellow wireframe.
            r.state.color    = Color::Yellow;
            r.state.fillMode = FillMode::WireFrame;
            r.drawAABB( translatedAABB );

            // Draw the collision Circle as a yellow wireframe.
            r.drawCircle( translatedCircle );
        }

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