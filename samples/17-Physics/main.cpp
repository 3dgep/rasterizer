#include "Ball.hpp"

#include <Timer.hpp>

#include <graphics/Image.hpp>
#include <graphics/Rasterizer.hpp>
#include <graphics/Window.hpp>

using namespace sr;

constexpr int SCREEN_WIDTH  = 800;
constexpr int SCREEN_HEIGHT = 600;

int main()
{
    Window     window( "Clear Screen", SCREEN_WIDTH, SCREEN_HEIGHT );
    Image      image { SCREEN_WIDTH, SCREEN_HEIGHT };
    Rasterizer rasterizer;
    Timer      timer;
    Text       fpsText { Font::DefaultFont, "FPS: 0" };

    // Setup the rasterizer's render target state.
    rasterizer.state.colorTarget = &image;

    std::vector<Ball> balls;
    balls.emplace_back( glm::vec2 { 300.0f, 0.0f }, 16.0f, 16.0f );
    balls.emplace_back( glm::vec2 { 320.0f, SCREEN_HEIGHT - 50.0f }, 50.0f, 50.0f );

    constexpr glm::vec2 GRAVITY { 0.0f, 400.0f };  // Force due to gravity in pixels/second.
    constexpr float     RESTITUTION = 0.8f;        // Bounciness of the balls. 1.0f is a perfect bounce.

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
                case SDLK_R:
                    balls.clear();
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
                }
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                glm::vec2 m = window.clientToImage( event.button.x, event.button.y, image );  // Convert mouse coords to image space.
                switch ( event.button.button )
                {
                case 1:  // Left mouse button
                    balls.emplace_back( m, 16.0f, 16.0f );
                    break;
                case 3:  // Right mouse button
                    balls.emplace_back( m, 50.0f, 50.0f );
                    break;
                }
            }
        }

        for ( auto& ball: balls )
        {
            // Apply gravity.
            ball.addAcceleration( GRAVITY );

            // Update
            ball.update( static_cast<float>( timer.elapsedSeconds() ) );

            // Check collisions
            glm::vec2 p = ball.getPosition();
            glm::vec2 v = ball.getVelocity();
            float     r = ball.getRadius();

            // Test collision with the bottom of the screen.
            if ( p.y + r >= SCREEN_HEIGHT - 1 )
            {
                p.y = ( SCREEN_HEIGHT - 1 ) - r;
                v.x *= RESTITUTION;
                v.y *= -RESTITUTION;
            }

            // Test collision with the left side of the screen.
            if ( p.x - r <= 0 )
            {
                p.x = r;
                v.x *= -RESTITUTION;
            }

            // Test collision with the right side of the screen.
            if ( p.x + r >= SCREEN_WIDTH - 1 )
            {
                p.x = ( SCREEN_WIDTH - 1 ) - r;
                v.x *= -0.5f;
            }

            ball.setPosition( p );
            ball.setVelocity( v );
            ball.setAcceleration( glm::vec2 { 0 } );
        }

        for ( size_t i = 0; i < balls.size(); i++ )
        {
            for ( size_t j = i + 1; j < balls.size(); j++ )
            {
                if ( auto hit = balls[i].checkCollision( balls[j] ) )
                {
                    if ( hit->depth > 0 )
                    {
                        // Correct the positions so the balls are no longer colliding.
                        glm::vec2 correction = hit->normal * hit->depth * 0.5f;
                        balls[i].setPosition( balls[i].getPosition() - correction );
                        balls[j].setPosition( balls[j].getPosition() + correction );
                    }

                    // Compute relative velocity along the normal.
                    glm::vec2 relVel         = balls[j].getVelocity() - balls[i].getVelocity();
                    float     velAlongNormal = glm::dot( relVel, hit->normal );

                    // Do not resolve if balls are moving away from each other.
                    if ( velAlongNormal > 0 )
                        continue;

                    // Compute restitution.
                    float impulse = -( 1.0f + RESTITUTION ) * velAlongNormal;
                    impulse /= 1.0f / balls[i].getMass() + 1.0f / balls[j].getMass();

                    // Apply impulse along normal
                    glm::vec2 impulseVector = hit->normal * impulse;
                    balls[i].setVelocity( balls[i].getVelocity() - impulseVector * ( 1.0f / balls[i].getMass() ) );
                    balls[j].setVelocity( balls[j].getVelocity() + impulseVector * ( 1.0f / balls[j].getMass() ) );
                }
            }
        }

        image.clear( Color::Black );

        for ( auto& ball: balls )
        {
            ball.draw( rasterizer );
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