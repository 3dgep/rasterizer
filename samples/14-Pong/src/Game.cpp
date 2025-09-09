#include <Game.hpp>

#include <MenuState.hpp>
#include <Scores.hpp>

using namespace sr::graphics;

Game::Game( int screenWidth, int screenHeight )
{
    Scores::loadScores( "pong_scores.txt" );
    m_GameState = new MenuState( screenWidth, screenHeight );
    m_GameState->beginState();
}

Game::~Game()
{
    Scores::saveScores( "pong_scores.txt" );
    delete m_GameState;
}

void Game::update( float deltaTime )
{
    if (State* newState = m_GameState->update( deltaTime ))
    {
        delete m_GameState;
        m_GameState = newState;
        m_GameState->beginState();
    }
}

void Game::draw( Rasterizer& rasterizer )
{
    m_GameState->draw( rasterizer );
}
