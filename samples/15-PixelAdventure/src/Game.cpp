#include <Game.hpp>

#include <graphics/Color.hpp>
#include <input/Input.hpp>

#include <format>
#include <string>

using namespace sr;
using namespace input;

Game::Game( uint32_t screenWidth, uint32_t screenHeight )
: image { screenWidth, screenHeight }
, arial20 { "assets/fonts/arial.ttf", 20 }
, arial24 { "assets/fonts/arial.ttf", 24 }
{

    rasterizer.state.colorTarget = &image;
    rasterizer.state.cullMode    = CullMode::None;  // Disable culling.

    // Input that controls the characters horizontal movement.
    Input::addAxisCallback( "Horizontal", []( std::span<const GamepadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        float leftX = 0.0f;

        for ( auto& gamePadState: gamePadStates )
        {
            const auto state = gamePadState.getLastState();

            leftX += state.thumbSticks.leftX;
        }

        const auto keyState = keyboardState.getLastState();

        const float a     = keyState.A ? 1.0f : 0.0f;
        const float d     = keyState.D ? 1.0f : 0.0f;
        const float left  = keyState.Left ? 1.0f : 0.0f;
        const float right = keyState.Right ? 1.0f : 0.0f;

        return std::clamp( leftX - a + d - left + right, -1.0f, 1.0f );
    } );

    // Input that controls jumping.
    Input::addButtonDownCallback( "Jump", []( std::span<const GamepadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        bool a = false;

        for ( auto& gamePadState: gamePadStates )
        {
            a = a || gamePadState.a == ButtonState::Pressed;
        }

        using Keyboard::Key;
        const bool space = keyboardState.isKeyPressed( Key::Space );
        const bool up    = keyboardState.isKeyPressed( Key::Up );
        const bool w     = keyboardState.isKeyPressed( Key::W );

        return a || space || up || w;
    } );

    // Input to go to the next map.
    Input::addButtonDownCallback( "Next", []( std::span<const GamepadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        bool start = false;

        for ( auto& gamePadState: gamePadStates )
        {
            start = start || gamePadState.start == ButtonState::Pressed;
        }

        return start;
    } );

    // Input to go to the previous map.
    Input::addButtonDownCallback( "Previous", []( std::span<const GamepadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        bool back = false;

        for ( auto& gamePadState: gamePadStates )
        {
            back = back || gamePadState.back == ButtonState::Pressed;
        }

        return back;
    } );

    // Input to go to reload the current map.
    Input::addButtonDownCallback( "Reload", []( std::span<const GamepadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        bool b = false;

        for ( auto& gamePadState: gamePadStates )
        {
            b = b || gamePadState.b == ButtonState::Pressed;
        }

        const bool enter = keyboardState.isKeyPressed( Keyboard::Key::Enter );

        return b || enter;
    } );

    backgrounds.emplace_back( Background { "assets/Pixel Adventure/Background/Blue.png", 1.0f, { 0.0f, 1.0f }, 0.3f } );
    backgrounds.emplace_back( Background { "assets/Pixel Adventure/Background/Brown.png", 1.0f, { 0.0f, 1.0f }, 0.3f } );
    backgrounds.emplace_back( Background { "assets/Pixel Adventure/Background/Gray.png", 1.0f, { 0.0f, 1.0f }, 0.3f } );
    backgrounds.emplace_back( Background { "assets/Pixel Adventure/Background/Green.png", 1.0f, { 0.0f, 1.0f }, 0.3f } );
    backgrounds.emplace_back( Background { "assets/Pixel Adventure/Background/Pink.png", 1.0f, { 0.0f, 1.0f }, 0.3f } );
    backgrounds.emplace_back( Background { "assets/Pixel Adventure/Background/Purple.png", 1.0f, { 0.0f, 1.0f }, 0.3f } );
    backgrounds.emplace_back( Background { "assets/Pixel Adventure/Background/Yellow.png", 1.0f, { 0.0f, 1.0f }, 0.3f } );

    // The loadLevel function will switch to the next level.
    // Setting the current background to the last background ensures
    // the first background is used when the first level is loaded.
    currentBackground = backgrounds.end() - 1;

    project.loadFromFile( "assets/Pixel Adventure/Pixel Adventure.ldtk" );

    loadLevel( 0, 0 );

    transition = Transition( "assets/Pixel Adventure/Other/Transition.png" );

    // Buttons
    {
        // Previous button.
        SpriteSheet sheet { "assets/Pixel Adventure/Menu/Buttons/Previous.png", {}, {}, 0, 0, BlendMode::AlphaDiscard };
        previousButton = Button { sheet };
        previousButton.setCallback( [this] {
            onPreviousClicked();
        } );
        // Initially, we are on the first level, so hide the previous button.
        previousButton.enable( false );
    }
    {
        // Next button.
        SpriteSheet sheet { "assets/Pixel Adventure/Menu/Buttons/Next.png", {}, {}, 0, 0, BlendMode::AlphaDiscard };
        nextButton = Button { sheet };
        nextButton.setCallback( [this] {
            onNextClicked();
        } );
    }
    {
        // Restart button.
        SpriteSheet sheet { "assets/Pixel Adventure/Menu/Buttons/Restart.png", {}, {}, 0, 0, BlendMode::AlphaDiscard };
        restartButton = Button { sheet };
        restartButton.setCallback( [this] {
            onRestartClicked();
        } );
    }
}

void Game::update()
{
    if ( paused )
        return;

    timer.tick();

    // Update and draw the background.
    // image.clear( Color::Black );
    currentBackground->update( timer );
    currentBackground->draw( rasterizer );

    auto elapsedTime = static_cast<float>( timer.elapsedSeconds() );
    do
    {
        // Update the input state.
        Input::update();

        // Check if next/previous input buttons have been pressed.
        if ( Input::getButtonDown( "Next" ) )
        {
            onNextClicked();
        }
        if ( Input::getButtonDown( "Previous" ) )
        {
            onPreviousClicked();
        }
        if ( Input::getButtonDown( "Reload" ) )
        {
            onRestartClicked();
        }

        currentLevel.update( std::min( elapsedTime, physicsTick ) );
        elapsedTime -= physicsTick;
    } while ( elapsedTime > 0.0f );

    // Check to see if the player died
    if ( currentLevel.getPlayer().isDead() )
    {
        // Reload the level.
        onRestartClicked();
    }

    currentLevel.draw( rasterizer );

    // Draw the buttons
    restartButton.draw( rasterizer );
    nextButton.draw( rasterizer );
    previousButton.draw( rasterizer );

    // Update the transition effect.
    switch ( transitionState )
    {
    case TransitionState::None:
        transitionTime = 0.0f;
        break;
    case TransitionState::In:
        transitionTime += static_cast<float>( timer.elapsedSeconds() );
        if ( transitionTime > transitionDuration )
        {
            loadLevel( nextLevelId, ++currentCharacterId );
            transitionState = TransitionState::Out;
        }
        break;
    case TransitionState::Out:
        transitionTime -= static_cast<float>( timer.elapsedSeconds() );
        if ( transitionTime < 0.0f )
            transitionState = TransitionState::None;
        break;
    }

    if ( transitionState != TransitionState::None )
    {
        transition.setRatio( transitionTime / transitionDuration );
        transition.draw( rasterizer );
    }

#if _DEBUG
    {
        drawFPS();

        // Draw some text at the mouse position.
        int x = static_cast<int>( mousePos.x );
        int y = static_cast<int>( mousePos.y );

        std::string text = std::format( "({}, {})", x, y );

        // Copy rasterizer state
        auto r        = rasterizer;
        r.state.color = Color::Black;
        r.drawText( arial20, text, x + 2, y + 2 );
        r.state.color = Color::White;
        r.drawText( arial20, text, x, y );
    }
#endif

    //    timer.limitFPS( 30 );
}

void Game::processEvent( const SDL_Event& _event )
{
    // Copy the event so we can modify it.
    SDL_Event event = _event;

    switch ( event.type )
    {
    case SDL_EVENT_KEY_DOWN:
        switch ( event.key.key )
        {
        case SDLK_P:
            paused = !paused;
            break;
    case SDLK_S:
            if ( event.key.mod & SDL_KMOD_CTRL )
            {
                image.save( "screenshot.png" );
                std::cout << "Screenshot saved to: " << std::filesystem::current_path() / "screenshot.png" << std::endl;
            }
            break;
        }
        break;
    case SDL_EVENT_MOUSE_MOTION:
        onMouseMoved( event.motion );
        break;
    case SDL_EVENT_WINDOW_RESIZED:
        onResized( event.window );
        break;
    }

    previousButton.processEvents( event );
    nextButton.processEvents( event );
    restartButton.processEvents( event );
}

void Game::onMouseMoved( SDL_MouseMotionEvent& args )
{
    // Compute the mouse position relative to the game screen (which can be scaled if the window is resized).
    const glm::vec2 scale {
        static_cast<float>( image.getWidth() ) / static_cast<float>( gameRect.width ),
        static_cast<float>( image.getHeight() ) / static_cast<float>( gameRect.height )
    };
    const glm::vec2 offset { gameRect.topLeft() };

    args.x = ( args.x - offset.x ) * scale.x;
    args.y = ( args.y - offset.y ) * scale.y;

    mousePos = { args.x, args.y };
}

void Game::onResized( SDL_WindowEvent& args )
{
    if ( args.type != SDL_EVENT_WINDOW_RESIZED )
        return;

    const float aspectRatio = static_cast<float>( image.getWidth() ) / static_cast<float>( image.getHeight() );
    const float scaleWidth  = static_cast<float>( args.data1 ) / static_cast<float>( image.getWidth() );
    const float scaleHeight = static_cast<float>( args.data2 ) / static_cast<float>( image.getHeight() );

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

    // Position the buttons in the top-right corner of the game screen.
    constexpr float spacing = 3.0f;
    float           y       = spacing;
    float           x       = static_cast<float>( image.getWidth() ) - spacing;

    x -= restartButton.getWidth() + spacing;
    restartButton.setTransform( Transform2D { { x, y } } );

    x -= nextButton.getWidth() + spacing;
    nextButton.setTransform( Transform2D { { x, y } } );

    x -= previousButton.getWidth() + spacing;
    previousButton.setTransform( Transform2D { { x, y } } );
}

void Game::onPreviousClicked()
{
    std::cout << "Previous Clicked!" << std::endl;

    if ( transitionState != TransitionState::None )
        return;

    if ( currentLevelId > 0 )
    {

        transitionState = TransitionState::In;
        transitionTime  = 0.0f;

        nextLevelId = currentLevelId - 1;
    }
}

void Game::onNextClicked()
{
    std::cout << "Next Clicked!" << std::endl;

    if ( transitionState != TransitionState::None )
        return;

    transitionState = TransitionState::In;
    transitionTime  = 0.0f;

    nextLevelId = currentLevelId + 1;
}

void Game::onRestartClicked()
{
    std::cout << "Restart Clicked!" << std::endl;

    if ( transitionState != TransitionState::None )
        return;

    transitionState = TransitionState::In;
    transitionTime  = 0.0f;
}

void Game::loadLevel( size_t levelId, size_t characterId )
{
    auto& world  = project.getWorld();
    auto& levels = world.allLevels();

    currentLevelId = levelId % levels.size();

    currentLevel = Level { project, world, levels[currentLevelId] };

    currentLevel.setCharacter( characterId );

    previousButton.enable( currentLevelId != 0 );

    // Also change the background
    if ( ++currentBackground == backgrounds.end() )
    {
        currentBackground = backgrounds.begin();
    }

    transitionState = TransitionState::Out;
}

void Game::drawFPS()
{
    static double      totalTime = 0.0;
    static uint64_t    frames    = 0;
    static std::string fps       = "FPS: 0";

    ++frames;
    totalTime += timer.elapsedSeconds();
    if ( totalTime > 1.0 )
    {
        fps       = std::format( "FPS: {:.3f}", static_cast<double>( frames ) / totalTime );
        frames    = 0;
        totalTime = 0.0;
    }

    auto r        = rasterizer;
    r.state.color = Color::Black;
    r.drawText( arial20, fps, 6, 6 );
    r.state.color = Color::White;
    r.drawText( arial20, fps, 4, 4 );
}
