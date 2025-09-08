#pragma once

#include "Ball.hpp"

#include <graphics/Rasterizer.hpp>
#include <graphics/Text.hpp>

class Menu
{
public:
    Menu() = default;

    Menu( int screenWidth, int screenHeight );

    void update( float deltaTime );

    void draw( sr::Rasterizer& rasterizer );

private:
    int m_ScreenWidth = 0;
    int m_ScreenHeight = 0;

    Ball m_Ball;

    sr::Font m_ScoreFont;
    sr::Text m_P1Score;
    sr::Text m_P2Score;
};