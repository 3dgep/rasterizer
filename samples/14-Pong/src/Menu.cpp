#include <Menu.hpp>

using namespace sr::graphics;

Menu::Menu( int screenWidth, int screenHeight )
: m_ScreenWidth { screenWidth }
, m_ScreenHeight { screenHeight }
, m_ScoreFont { "assets/fonts/pong-score/pong-score.otf", 40.0f }
, m_P1Score { m_ScoreFont, "1" }
, m_P2Score { m_ScoreFont, "1" }
{
    m_Ball.setPosition( { m_ScreenWidth / 2, m_ScreenHeight / 2 } );

    glm::vec2 v = normalize(glm::vec2{ 1, -2 }) * 260.0f;
    m_Ball.setVelocity( v );
}

void Menu::update( float deltaTime )
{
    m_Ball.update( deltaTime );

    // Check for collision with the sides of the screen.
    auto p = m_Ball.getPosition();
    auto v = m_Ball.getVelocity();
    auto aabb = m_Ball.getAABB();

    if (aabb.left() <= 0)
    {
        p.x -= aabb.left();
        v.x *= -1.0f;
    }
    else if (aabb.right() >= m_ScreenWidth)
    {
        p.x += m_ScreenWidth - aabb.right() - 1;
        v.x *= -1.0f;
    }

    if (aabb.top() <= 0)
    {
        p.y -= aabb.top();
        v.y *= -1.0f;
    }
    else if (aabb.bottom() >= m_ScreenHeight)
    {
        p.y += m_ScreenHeight - aabb.bottom() - 1;
        v.y *= -1.0f;
    }

    m_Ball.setPosition( p );
    m_Ball.setVelocity( v );

}

void Menu::draw( sr::Rasterizer& rasterizer )
{
    rasterizer.drawText( m_P1Score,  m_ScreenWidth / 4, 10 );
    rasterizer.drawText( m_P1Score, 3 * m_ScreenWidth / 4, 10 );

    const int lineWidth = 4;
    const int x         = m_ScreenWidth / 2;

    // Draw the stippled middle line.
    for (int y = 0; y < m_ScreenHeight; y += lineWidth * 2)
    {
        rasterizer.drawLine( x, y, x, y + lineWidth - 1 );
    }

    m_Ball.draw( rasterizer );
}