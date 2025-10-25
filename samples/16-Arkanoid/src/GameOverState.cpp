#include "GameOverState.hpp"

using namespace sr;

GameOverState::GameOverState( Game& _game )
: game { _game }
, gameOverText{game.getFont(), "GAME OVER"}
{}

void GameOverState::update( float deltaTime )
{
    timer += deltaTime;

    if ( timer > timeout )
        game.setNextState( Game::GameState::MainMenu );
}

void GameOverState::draw( Rasterizer& rasterizer )
{
    rasterizer.drawText( gameOverText, 77, 123 );
}
