#include "Audio/Device.hpp"

#include <Timer.hpp>

#include <graphics/Image.hpp>
#include <graphics/Rasterizer.hpp>
#include <graphics/Window.hpp>

#include <Audio/Sound.hpp>

#include <random>

using namespace sr;

int main()
{
    Window      window( "Clear Screen", 800, 600 );
    Image       image { 800, 600 };
    Rasterizer  rasterizer;
    Timer       timer;
    Text        fpsText { Font::DefaultFont, "FPS: 0" };
    Sprite      ball { "assets/textures/ball.png", BlendMode::AlphaDiscard };
    Transform2D transform { { image.getWidth() / 2, image.getHeight() / 2 } };
    transform.setAnchor( { ball.getWidth() / 2, ball.getHeight() / 2 } );
    glm::vec2 velocity = glm::vec2 { 1, -1 } * 200.0f;

    // Load a bunch of bouncy sounds.
    Audio::Sound bounceSounds[] = {
        Audio::Sound { "assets/sounds/bounce-1.wav" },
        Audio::Sound { "assets/sounds/bounce-2.wav" },
        Audio::Sound { "assets/sounds/bounce-3.wav" },
        Audio::Sound { "assets/sounds/bounce-4.wav" }
    };

    // Load some looping background music
    Audio::Sound bgMusic { "assets/sounds/Sweet Treats.ogg", Audio::Sound::Type::Music };
    // Turn down the volume.
    bgMusic.setVolume( 0.2f );
    bgMusic.setLooping( true );
    bgMusic.play();

    // Set the attenuation model to Linear (the default is Inverse)
    for (auto& s : bounceSounds)
    {
        s.setAttenuationModel( Audio::Sound::AttenuationModel::Linear );
    }

    // Some RNG to pick a random bounce sound to play.
    std::minstd_rand rng { std::random_device()() };
    std::uniform_int_distribution<> dist { 0, std::size( bounceSounds ) - 1 };

    // Set the Audio listener to be in the center of the screen.
    Audio::Device::getListener().setPosition( { image.getWidth() / 2.0f, image.getHeight() / 2.0f, 0.0f } );

    // Setup the rasterizer's render target state.
    rasterizer.state.colorTarget = &image;

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
                case SDLK_RETURN:
                    if ( ( event.key.mod & SDL_KMOD_ALT ) != 0 )
                    {
                    case SDLK_F11:
                        window.toggleFullscreen();
                    }
                    break;
                }
                break;
            }
        }

        // Move the ball
        {
            glm::vec2 p = transform.getPosition();
            p += velocity * static_cast<float>( timer.elapsedSeconds() );
            Circle c { p, ball.getWidth() / 2.0f };
            if (c.left() <= 0.0f)
            {
                c.center.x = c.radius;
                velocity.x *= -1.0f;
                // Play a random 1-shot bounce sound effect.
                bounceSounds[dist( rng )].replay();
            }
            else if (c.right() >= image.getWidth())
            {
                c.center.x = image.getWidth() - c.radius;
                velocity.x *= -1;
                // Play a random 1-shot bounce sound effect.
                bounceSounds[dist( rng )].replay();
            }

            if (c.top() <= 0.0f)
            {
                c.center.y = c.radius;
                velocity.y *= -1.0f;
                // Play a random 1-shot bounce sound effect.
                bounceSounds[dist( rng )].replay();
            }
            else if (c.bottom() >= image.getHeight())
            {
                c.center.y = image.getHeight() - c.radius;
                velocity.y *= -1.0f;
                // Play a random 1-shot bounce sound effect.
                bounceSounds[dist( rng )].replay();
            }

            transform.setPosition( c.center );

            // Update sound positions
            for (auto& s : bounceSounds)
            {
                s.setPosition( { c.center.x, c.center.y } );
            }

        }

        image.clear( Color::Black );

        rasterizer.drawSprite( ball, transform );

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