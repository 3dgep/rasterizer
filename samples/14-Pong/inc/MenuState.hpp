#pragma once

#include "Ball.hpp"
#include "State.hpp"

#include <graphics/Rasterizer.hpp>
#include <graphics/Text.hpp>

class MenuState : public State
{
public:
    MenuState() = default;

    MenuState( int screenWidth, int screenHeight );

    void beginState() override;

    State* update( float deltaTime ) override;

    void draw( sr::Rasterizer& rasterizer ) override;

private:
    int m_ScreenWidth = 0;
    int m_ScreenHeight = 0;

    Ball m_Ball;

    sr::Font m_ScoreFont;
    sr::Text m_P1Score;
    sr::Text m_P2Score;
};