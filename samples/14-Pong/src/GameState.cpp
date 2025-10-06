#include "MenuState.hpp"

#include <GameState.hpp>
#include <Scores.hpp>
#include <Sound.hpp>

#include <input/Input.hpp>

#include <algorithm>
#include <cmath>
#include <random>

using namespace input;
using namespace sr::math;

using Keyboard::Key;

namespace
{
std::random_device rd;
std::minstd_rand   rng( rd() );
}  // namespace

GameState::GameState( int screenWidth, int screenHeight )
: StateBase( screenWidth, screenHeight )
, m_GameOverText( m_ScoreFont, "GAME OVER" )
{
    // Controls for the left paddle.
    Input::addAxisCallback( "P1", []( std::span<const GamepadStateTracker> gamepadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
#if _DEBUG
        float leftY  = 0.0f;
        float rightY = 0.0f;
        if ( gamepadStates[1].getLastState().connected )
        {
            leftY  = gamepadStates[0].getLastState().thumbSticks.leftY;
            rightY = gamepadStates[0].getLastState().thumbSticks.rightY;
        }
        else
        {
            leftY = gamepadStates[0].getLastState().thumbSticks.leftY;
        }
#else
        float leftY  = gamepadStates[0].getLastState().thumbSticks.leftY;
        float rightY = gamepadStates[0].getLastState().thumbSticks.rightY;
#endif

        float w = keyboardState.lastState.W ? 1.0f : 0.0f;
        float s = keyboardState.lastState.S ? 1.0f : 0.0f;

        return std::clamp( s - w + leftY + rightY, -1.0f, 1.0f );
    } );

    // Controls for the right paddle.
    Input::addAxisCallback( "P2", []( std::span<const GamepadStateTracker> gamepadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
#if _DEBUG
        float leftY = 0.0f;
        float rightY = 0.0f;
        if ( gamepadStates[1].getLastState().connected )
        {
            leftY = gamepadStates[1].getLastState().thumbSticks.leftY;
            rightY      = gamepadStates[1].getLastState().thumbSticks.rightY;
        }
        else
        {
            rightY = gamepadStates[0].getLastState().thumbSticks.rightY;
        }
#else
        float leftY  = gamepadStates[1].getLastState().thumbSticks.leftY;
        float rightY = gamepadStates[1].getLastState().thumbSticks.rightY;
#endif
        float up   = keyboardState.lastState.Up ? 1.0f : 0.0f;
        float down = keyboardState.lastState.Down ? 1.0f : 0.0f;

        return std::clamp( down - up + leftY + rightY, -1.0f, 1.0f );
    } );

    // Default AI difficulty is Medium
    setAIDifficulty( AIDifficulty::Medium );
}

void GameState::setAIDifficulty( AIDifficulty difficulty )
{
    m_AIDifficulty = difficulty;

    // Configure AI parameters based on difficulty
    switch ( difficulty )
    {
    case AIDifficulty::None:
        m_AIReactionDelay = 0.0f;
        m_AIErrorMargin   = 0.0f;
        break;
    case AIDifficulty::Easy:
        m_AIReactionDelay = 0.2f;   // 200ms delay
        m_AIErrorMargin   = 8.0f;  // Can be off by 8 pixels
        break;
    case AIDifficulty::Medium:
        m_AIReactionDelay = 0.1f;   // 100ms delay
        m_AIErrorMargin   = 6.0f;   // Can be off by 6 pixels
        break;
    case AIDifficulty::Hard:
        m_AIReactionDelay = 0.05f;  // 50ms delay
        m_AIErrorMargin   = 4.0f;   // Can be off by 4 pixels
        break;
    }
}

GameState::AIDifficulty GameState::getAIDifficulty() const noexcept
{
    return m_AIDifficulty;
}

void GameState::beginState()
{
    Scores::resetScores();

    m_P1Score = std::format( "{}", Scores::getP1Score() );
    m_P2Score = std::format( "{}", Scores::getP2Score() );

    m_Ball.setPosition( { m_ScreenWidth / 2, m_ScreenHeight / 2 } );

    m_P1Paddle.setY( m_ScreenHeight / 2 );
    m_P2Paddle.setY( m_ScreenHeight / 2 );

    std::bernoulli_distribution d( 0.5 );  // 50% chance who gets to serve first.

    setState( d( rng ) ? State::P1Serve : State::P2Serve );
}

State* GameState::update( float deltaTime )
{
    switch ( m_State )
    {
    case State::P1Serve:
    case State::P2Serve:
        updateServe( deltaTime );
        break;
    case State::Play:
        updatePlay( deltaTime );
        break;
    case State::GameOver:
        updateGameOver( deltaTime );
        break;
    case State::None:
        return new MenuState( m_ScreenWidth, m_ScreenHeight );
    }

    // At any time, pressing "Cancel" will return to the main manu.
    // "Cancel" is mapped to the "esc" key and the right face button ("B" button on an XBox controller).
    if ( Input::getButtonUp( "Cancel" ) )
    {
        return new MenuState( m_ScreenWidth, m_ScreenHeight );
    }

    return nullptr;
}

void GameState::draw( sr::graphics::Rasterizer& rasterizer )
{
    StateBase::draw( rasterizer );

    // Paddles are always drawn.
    m_P1Paddle.draw( rasterizer );
    m_P2Paddle.draw( rasterizer );

    switch ( m_State )
    {
    case State::P1Serve:
    case State::P2Serve:
        drawServe( rasterizer );
        break;
    case State::Play:
        drawPlay( rasterizer );
        break;
    case State::GameOver:
        drawGameOver( rasterizer );
        break;
    }
}

void GameState::setState( State newState )
{
    if ( m_State != newState )
    {
        startState( newState );
        m_State = newState;
    }
}

void GameState::startState( State state )
{
    switch ( state )
    {
    case State::P1Serve:
    case State::P2Serve:
        startServe( state );
        break;
    case State::Play:
        break;
    case State::GameOver:
        m_TotalTime = 0.0f;
    }
}

void GameState::startServe( State state )
{
    // Reset the serve timer.
    m_TotalTime = 0.0f;

    // Choose a random point to start the ball.
    std::uniform_int_distribution startY { m_Ball.getRadius(), m_ScreenHeight - m_Ball.getRadius() - 1 };
    int                           y = startY( rng );

    m_Ball.setPosition( { m_ScreenWidth / 2, y } );

    // The chance of the ball moving up is determined by the starting height of the ball.
    // If the ball is in the top-half of the screen, then there is a 25% chance the ball will start moving up.
    // If the ball is in the bottom-half of the screen, there is a 75% chance the ball will start moving up.
    std::bernoulli_distribution goUp( y < m_ScreenHeight / 2 ? 0.25 : 0.75 );

    // Go right if it's P1's serve, go left if it's P2's serve.
    float vx = ( state == State::P1Serve ) ? 1.0f : -1.0f;
    float vy = goUp( rng ) ? -2.0f : 2.0f;

    glm::vec2 velocity = glm::normalize( glm::vec2 { vx, vy } ) * 260.0f;

    m_Ball.setVelocity( velocity );
}

void GameState::updateServe( float deltaTime )
{
    updatePaddle( m_P1Paddle, "P1", deltaTime );
    // Use AI for player 2 if AI is enabled, otherwise use player input
    if ( m_AIDifficulty != AIDifficulty::None )
    {
        updateAIPaddle( m_P2Paddle, deltaTime );
    }
    else
    {
        updatePaddle( m_P2Paddle, "P2", deltaTime );
    }

    m_TotalTime += deltaTime;
    if ( m_TotalTime > 3.0f )  // After 3 seconds... play.
    {
        setState( State::Play );
    }
}

void GameState::updatePlay( float deltaTime )
{
    updatePaddle( m_P1Paddle, "P1", deltaTime );

    // Use AI for player 2 if AI is enabled, otherwise use player input
    if ( m_AIDifficulty != AIDifficulty::None )
    {
        updateAIPaddle( m_P2Paddle, deltaTime );
    }
    else
    {
        updatePaddle( m_P2Paddle, "P2", deltaTime );
    }

    m_Ball.update( deltaTime );

    // Check collisions with paddles.
    if ( m_P1Paddle.checkCollision( m_Ball ) || m_P2Paddle.checkCollision( m_Ball ) )
    {
        Sound::PaddleSound.play();
    }

    // Check collisions with walls.
    checkWallCollisions();

    if ( Scores::getP1Score() == 10 || Scores::getP2Score() == 10 )
    {
        setState( State::GameOver );
    }
}

void GameState::updateGameOver( float deltaTime )
{
    m_TotalTime += deltaTime;
    if ( m_TotalTime > 3.0f )  // wait 3 seconds...
    {
        setState( State::None );
    }
}

void GameState::updatePaddle( Paddle& paddle, std::string_view input, float deltaTime )
{
    float y        = paddle.getY();
    float initialY = y;

    y += Input::getAxis( input ) * deltaTime * 260.0f;
    paddle.setY( y );

    AABB aabb = paddle.getAABB();

    if ( aabb.top() < 0.0f )
    {
        y = aabb.height() / 2;
    }
    else if ( aabb.bottom() >= m_ScreenHeight )
    {
        y = m_ScreenHeight - 1 - aabb.height() / 2;
    }

    paddle.setY( y );

    float velY = deltaTime > 0.0f ? ( y - initialY ) / deltaTime : 0.0f;
    paddle.setVelocity( glm::vec2 { 0, velY } );
}

void GameState::updateAIPaddle( Paddle& paddle, float deltaTime )
{
    float y        = paddle.getY();
    float initialY = y;

    // Get ball information
    glm::vec2 ballPos = m_Ball.getPosition();
    glm::vec2 ballVel = m_Ball.getVelocity();

    // Target position for the AI paddle
    float targetY = ballPos.y;

    // Add some error margin to make AI more realistic
    if ( m_AIErrorMargin > 0.0f )
    {
        // Use a deterministic offset based on ball position
        float offset = std::sin( ballPos.x * 0.1f + ballPos.y * 0.1f ) * m_AIErrorMargin;
        targetY += offset;
    }

    // Only track the ball if it's moving towards the AI paddle (right side)
    if ( ballVel.x > 0.0f )
    {
        // Predict where the ball will be when it reaches the paddle
        float paddleX = paddle.getPosition().x;
        float ballX   = ballPos.x;
        float timeToReach = ( paddleX - ballX ) / ballVel.x;

        if ( timeToReach > 0.0f )
        {
            // Predict ball Y position
            float predictedY = ballPos.y + ballVel.y * timeToReach;

            // Account for wall bounces (simplified prediction)
            while ( predictedY < 0.0f || predictedY > m_ScreenHeight )
            {
                if ( predictedY < 0.0f )
                    predictedY = -predictedY;
                else if ( predictedY > m_ScreenHeight )
                    predictedY = 2.0f * m_ScreenHeight - predictedY;
            }

            targetY = predictedY;
        }
    }
    else
    {
        // If ball is moving away, center the paddle
        targetY = m_ScreenHeight / 2.0f;
    }

    // Calculate movement direction with reaction delay
    float diff = targetY - y;
    
    // Apply reaction delay - AI won't move if the difference is very small
    float reactionThreshold = m_AIReactionDelay * 100.0f;
    if ( std::abs( diff ) < reactionThreshold )
    {
        diff = 0.0f;
    }

    // Move towards target position
    float moveSpeed = 260.0f;  // Same speed as player
    
    // Scale movement based on difficulty (harder AI moves more precisely)
    if ( m_AIDifficulty == AIDifficulty::Easy )
    {
        moveSpeed *= 0.7f;  // 70% speed
    }
    else if ( m_AIDifficulty == AIDifficulty::Medium )
    {
        moveSpeed *= 0.85f;  // 85% speed
    }
    // Hard uses full speed

    if ( diff > 0.0f )
    {
        y += moveSpeed * deltaTime;
        if ( y > targetY )
            y = targetY;
    }
    else if ( diff < 0.0f )
    {
        y -= moveSpeed * deltaTime;
        if ( y < targetY )
            y = targetY;
    }

    paddle.setY( y );

    // Clamp to screen bounds
    AABB aabb = paddle.getAABB();

    if ( aabb.top() < 0.0f )
    {
        y = aabb.height() / 2;
    }
    else if ( aabb.bottom() >= m_ScreenHeight )
    {
        y = m_ScreenHeight - 1 - aabb.height() / 2;
    }

    paddle.setY( y );

    // Calculate velocity
    float velY = deltaTime > 0.0f ? ( y - initialY ) / deltaTime : 0.0f;
    paddle.setVelocity( glm::vec2 { 0, velY } );
}

void GameState::checkWallCollisions()
{
    auto p    = m_Ball.getPosition();
    auto v    = m_Ball.getVelocity();
    auto aabb = m_Ball.getAABB();

    if ( aabb.right() >= m_ScreenWidth )
    {
        Sound::PointSound.play();
        m_P1Score = std::format( "{}", Scores::addP1Score() );
        setState( State::P1Serve );

        return;
    }
    if ( aabb.left() <= 0 )
    {
        Sound::PointSound.play();
        m_P2Score = std::format( "{}", Scores::addP2Score() );
        setState( State::P2Serve );

        return;
    }

    if ( aabb.top() <= 0 )
    {
        p.y = m_Ball.getRadius();
        v.y *= -1.0f;
        Sound::WallSound.play();
    }
    else if ( aabb.bottom() >= m_ScreenHeight )
    {
        p.y = m_ScreenHeight - m_Ball.getRadius() - 1;
        v.y *= -1.0f;
        Sound::WallSound.play();
    }

    m_Ball.setPosition( p );
    m_Ball.setVelocity( v );
}

void GameState::drawServe( sr::graphics::Rasterizer& rasterizer )
{
    // Nothing extra needs to be drawn in this state.
}

void GameState::drawPlay( sr::graphics::Rasterizer& rasterizer )
{
    // Only the ball needs to be drawn in this state.
    m_Ball.draw( rasterizer );
}

void GameState::drawGameOver( sr::graphics::Rasterizer& rasterizer )
{
    // Draw "Game Over" in the center of the screen.
    rasterizer.drawText( m_GameOverText, ( m_ScreenWidth - m_GameOverText.getWidth() ) / 2, ( m_ScreenHeight - m_GameOverText.getHeight() ) / 2 );
}
