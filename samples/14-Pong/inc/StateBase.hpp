#pragma once

#include "Ball.hpp"
#include "Paddle.hpp"
#include "State.hpp"

class StateBase : public State
{
public:
    StateBase( int screenWidth = 0, int screenHeight = 0);

    /// <summary>
    /// Draws the object using the specified rasterizer.
    /// </summary>
    /// <param name="rasterizer">A reference to the Rasterizer used for rendering.</param>
    void draw( sr::graphics::Rasterizer& rasterizer ) override;

protected:
    int m_ScreenWidth  = 0;
    int m_ScreenHeight = 0;

    Ball   m_Ball;
    Paddle m_P1Paddle;
    Paddle m_P2Paddle;

    sr::Font m_ScoreFont;
    sr::Text m_P1Score;
    sr::Text m_P2Score;
};