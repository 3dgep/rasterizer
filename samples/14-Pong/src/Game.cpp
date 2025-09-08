#include <Game.hpp>

using namespace sr::graphics;

Game::Game( int screenWidth, int screenHeight )
: m_MainMenu { screenWidth, screenHeight }
{}

void Game::processEvents( const SDL_Event& event )
{
}

void Game::update( float deltaTime )
{
    switch ( m_GameState )
    {
    case State::Menu:
        updateMenu( deltaTime );
        break;
    case State::Game:
        break;
    case State::GameOver:
        break;
    }
}

void Game::draw( Rasterizer& rasterizer )
{
    switch ( m_GameState )
    {
    case State::Menu:
        drawMenu( rasterizer );
        break;
    case State::Game:
        break;
    case State::GameOver:
        break;
    }
}

void Game::updateMenu( float deltaTime )
{
    m_MainMenu.update( deltaTime );
}

void Game::drawMenu( sr::Rasterizer& rasterizer )
{
    m_MainMenu.draw( rasterizer );
}
