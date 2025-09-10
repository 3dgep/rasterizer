#include "GameState.hpp"

#include <MenuState.hpp>
#include <Scores.hpp>
#include <Sound.hpp>

#include <input/Input.hpp>

using namespace sr::input;
using namespace sr::graphics;

MenuState::MenuState( int screenWidth, int screenHeight )
    : StateBase( screenWidth, screenHeight )
{
    m_Ball.setPosition( { m_ScreenWidth / 2, m_ScreenHeight / 2 } );

    glm::vec2 v = normalize( glm::vec2 { 1, -2 } ) * 260.0f;
    m_Ball.setVelocity( v );
}

void MenuState::beginState()
{
    m_P1Score = std::format( "{}", Scores::getP1Score() );
    m_P2Score = std::format( "{}", Scores::getP2Score() );
}

State* MenuState::update( float deltaTime )
{
    m_Ball.update( deltaTime );

    // Check for collision with the sides of the screen.
    auto p    = m_Ball.getPosition();
    auto v    = m_Ball.getVelocity();
    auto aabb = m_Ball.getAABB();

    if ( aabb.left() <= 0 )
    {
        p.x -= aabb.left();
        v.x *= -1.0f;
        Sound::WallSound.play();
    }
    else if ( aabb.right() >= m_ScreenWidth )
    {
        p.x += m_ScreenWidth - aabb.right() - 1;
        v.x *= -1.0f;
        Sound::WallSound.play();
    }

    if ( aabb.top() <= 0 )
    {
        p.y -= aabb.top();
        v.y *= -1.0f;
        Sound::WallSound.play();
    }
    else if ( aabb.bottom() >= m_ScreenHeight )
    {
        p.y += m_ScreenHeight - aabb.bottom() - 1;
        v.y *= -1.0f;
        Sound::WallSound.play();
    }

    m_Ball.setPosition( p );
    m_Ball.setVelocity( v );

    // Switch to the game state with the "Submit" button is pressed.
    // This is mapped to the "Enter" key, the "Space" key, and the "A" button on any gamepad.
    if ( Input::getButtonUp( "Submit" ) )
    {
        return new GameState( m_ScreenWidth, m_ScreenHeight );
    }

    return nullptr;
}

void MenuState::draw( sr::Rasterizer& rasterizer )
{
    StateBase::draw( rasterizer );

    m_Ball.draw( rasterizer );
}