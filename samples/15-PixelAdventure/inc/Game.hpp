#pragma once

#include <Timer.hpp>
#include <graphics/Image.hpp>
#include <graphics/Rasterizer.hpp>
#include <graphics/Window.hpp>

class Game
{
public:
    Game(int screenWidth, int screenHeight );
    ~Game() = default;

    Game( const Game& )            = delete;
    Game( Game&& )                 = delete;
    Game& operator=( const Game& ) = delete;
    Game& operator=( Game&& )      = delete;

    void update();

private:
    Timer          m_Timer;
    sr::Window     m_Window;
    sr::Image      m_Image;
    sr::Rasterizer m_Rasterizer;
};