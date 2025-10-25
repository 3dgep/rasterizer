#include <Game.hpp>
#include <TitleState.hpp>

#include <graphics/ResourceManager.hpp>

#include <input/Input.hpp>

#include <format>

using namespace sr;
using namespace input;

TitleState::TitleState( Game& game )
: game { game }
, screenWidth( static_cast<int>( game.getImage().getWidth() ) )
, screenHeight( static_cast<int>( game.getImage().getHeight() ) )
{
    auto shipImage  = ResourceManager::loadImage( "assets/Arkanoid/ship.png" );
    auto taitoImage = ResourceManager::loadImage( "assets/Arkanoid/taito.png" );

    arkanoidSprite = Sprite( shipImage, RectI { 0, 0, 193, 42 }, BlendMode::AlphaDiscard );
    taitoSprite    = Sprite( taitoImage );
}

void TitleState::update( float deltaTime )
{
    if ( game.getCoins() > 0 && Input::getButtonDown( "Start 1" ) )
    {
        game.setNumPlayers( 1 );
        game.setNextState( Game::GameState::Playing );
    }
    else if ( game.getCoins() > 1 && Input::getButtonDown( "Start 2" ) )
    {
        game.setNumPlayers( 2 );
        game.setNextState( Game::GameState::Playing );
    }
}

void TitleState::draw( Rasterizer& rasterizer )
{
    rasterizer.clear( Color::Black );

    rasterizer.drawSprite( arkanoidSprite, 16, 51 );

    int         coins = game.getCoins();
    const auto& font  = game.getFont();

    if ( coins > 0 )
    {
        rasterizer.drawText( font, "PUSH", 97, 120 );
        if ( coins < 2 )
        {
            rasterizer.drawText( font, "ONLY 1 PLAYER BUTTON", 33, 144 );
        }
        else
        {
            rasterizer.drawText( font, "1 OR 2 PLAYER BUTTON", 34, 144 );
        }
    }

    rasterizer.drawSprite( taitoSprite, 64, 179 );
    rasterizer.drawText( font, L"� 1986 TAITO CORP JAPAN", 16, 208 );
    rasterizer.drawText( font, "ALL RIGHTS RESERVED", 33, 222 );
    rasterizer.drawText( font, std::format( "CREDIT{:3d}", coins ), 145, 255 );
}

void TitleState::processEvent( const SDL_Event& _event )
{

    // Copy the event so we can modify it.
    SDL_Event event = _event;

    switch ( event.type )
    {
    case SDL_EVENT_MOUSE_MOTION:
        onMouseMoved( event.motion );
        break;
    case SDL_EVENT_WINDOW_RESIZED:
        onResized( event.window );
        break;
    }
}

void TitleState::onMouseMoved( SDL_MouseMotionEvent& args )
{
    // Compute the mouse position relative to the menu screen (which can be scaled if the window is resized).
    const glm::vec2 scale {
        static_cast<float>( screenWidth ) / static_cast<float>( gameRect.width ),
        static_cast<float>( screenHeight ) / static_cast<float>( gameRect.height )
    };
    const glm::vec2 offset { gameRect.topLeft() };

    args.x = ( args.x - offset.x ) * scale.x;
    args.y = ( args.y - offset.y ) * scale.y;

    mousePos = { args.x, args.y };
}

void TitleState::onResized( const SDL_WindowEvent& args )
{
    if ( args.type != SDL_EVENT_WINDOW_RESIZED )
        return;

    const float aspectRatio = static_cast<float>( screenWidth ) / static_cast<float>( screenHeight );
    const float scaleWidth  = static_cast<float>( args.data1 ) / static_cast<float>( screenWidth );
    const float scaleHeight = static_cast<float>( args.data2 ) / static_cast<float>( screenHeight );

    int width;
    int height;

    if ( scaleWidth < scaleHeight )
    {
        // Size according to the width.
        width  = args.data1;
        height = static_cast<int>( static_cast<float>( width ) / aspectRatio );
    }
    else
    {
        // Size according to the height.
        height = args.data2;
        width  = static_cast<int>( static_cast<float>( height ) * aspectRatio );
    }

    gameRect = RectI {
        ( args.data1 - width ) / 2,
        ( args.data2 - height ) / 2,
        width, height
    };
}
