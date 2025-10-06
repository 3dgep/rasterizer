#include <Game.hpp>

Game::Game( int screenWidth, int screenHeight )
: m_Window( "Pixel Adventure", screenWidth, screenHeight, true )
, m_Image( screenWidth, screenHeight )
{
}
