#include <Scores.hpp>
#include <StateBase.hpp>

#include <format>

using namespace sr::graphics;

StateBase::StateBase( int screenWidth, int screenHeight )
: m_ScreenWidth { screenWidth }
, m_ScreenHeight { screenHeight }
, m_Ball { { screenWidth / 2, screenHeight / 2 }, { 0, 0 } }
, m_P1Paddle { { 3 * screenWidth / 20, screenHeight / 2 } }
, m_P2Paddle { { 17 * screenWidth / 20, screenHeight / 2 } }
, m_ScoreFont { "assets/fonts/pong-score-extended/pong-score-extended.otf", 20.0f }
, m_P1Score { m_ScoreFont, std::format( "{}", Scores::getP1Score() ) }
, m_P2Score { m_ScoreFont, std::format( "{}", Scores::getP2Score() ) }
{}

void StateBase::draw( Rasterizer& rasterizer )
{
    rasterizer.state.color = Color::White;

    rasterizer.drawText( m_P1Score, m_ScreenWidth / 4, 10 );
    rasterizer.drawText( m_P2Score, 3 * m_ScreenWidth / 4, 10 );

    constexpr int lineWidth = 4;
    const int     x         = m_ScreenWidth / 2;

    // Draw the stippled middle line.
    for ( int y = 0; y < m_ScreenHeight; y += lineWidth * 2 )
    {
        rasterizer.drawLine( x, y, x, y + lineWidth - 1 );
    }
}