#include <IntroState.hpp>
#include <ParseRects.hpp>

#include <graphics/ResourceManager.hpp>

#include <string>

using namespace sr;

static const std::string lines[] = {
    "THE ERA AND TIME OF\n"
    "THIS STORY IS UNKNOWN.",
    "AFTER THE MOTHERSHIP\n"
    "\"ARKANOID\" WAS DESTROYED,\n"
    "A SPACECRAFT \"VAUS\"\n"
    "SCRAMBLED AWAY FROM FROM IT.",
    "BUT ONLY TO BE\n"
    "TRAPPED IN SPACE WARPED\n"
    "BY SOMEONE........"
};

IntroState::IntroState( Game& game )
: game { game }
, starBackground { "assets/Arkanoid/stars.png" }
{
    auto spriteRects       = parseRects( "assets/Arkanoid/ship.xml" );
    auto spriteSheet       = ResourceManager::loadSpriteSheet( "assets/Arkanoid/ship.png", spriteRects, BlendMode::AlphaDiscard );
    shipAnimation          = SpriteAnimation { spriteSheet, 30, { { 0, 1 } } };
    shipDestroyedAnimation = SpriteAnimation { spriteSheet, 5, { { 2, 3, 4, 5, 6, 5, 4, 3 } } };
}

void IntroState::update( float deltaTime )
{
    time1 += deltaTime;
    time2 += deltaTime;

    character = static_cast<int>( time1 * cps );
    character = std::min<int>( character, lines[line].size() );  // Clamp to maximum characters in the current line.

    switch ( line )
    {
    case 0:
        if ( time1 > 2.5f )
        {
            ++line;
            time1 = 0.0f;
        }
        break;
    case 1:
        if ( time1 > 3.5f )
        {
            ++line;
            time1 = 0.0f;
        }
        break;
    case 2:
        if ( time1 > 2.6f )
        {
            game.setNextState( Game::GameState::Playing );
        }
        break;
    }

    // Update ship animations.
    if ( time2 < 2.0f )
        shipAnimation.update( deltaTime );
    else
        shipDestroyedAnimation.update( deltaTime );
}

void IntroState::draw( sr::graphics::Rasterizer& rasterizer )
{
    rasterizer.drawImage( starBackground, 0, 0 );

    if ( time2 < 2.0f )
        rasterizer.drawSprite( shipAnimation, 9, 153 );
    else
        rasterizer.drawSprite( shipDestroyedAnimation, 9, 153 );

    rasterizer.drawText( game.getFont(), lines[line].substr( 0, character ), 8, 30 );
}