#pragma once

#include <Timer.hpp>
#include <graphics/Image.hpp>
#include <graphics/Rasterizer.hpp>
#include <graphics/Window.hpp>

#include "Background.hpp"

#include <vector>

class Game
{
public:
    Game( int screenWidth, int screenHeight );
    ~Game() = default;

    Game( const Game& )            = delete;
    Game( Game&& )                 = delete;
    Game& operator=( const Game& ) = delete;
    Game& operator=( Game&& )      = delete;

    void loadLevel( size_t levelId );

    /// <summary>
    /// Update and draw the game.
    /// </summary>
    /// <returns>`true` to continue updating the game, `false` to terminate the application.</returns>
    bool update();

    // Button handlers.
    void onPrevious();
    void onNext();
    void onRestart();

private:
    // Maximum time for a physics tick.
    const float m_PhysicsTick = 1.0f / 60.0f;

    Timer          m_Timer;
    sr::Window     m_Window;
    sr::Image      m_Image;
    sr::Rasterizer m_Rasterizer;
    sr::Text       m_FPSText;

    size_t m_CurrentLevelId = 0u;

    // Backgrounds
    std::vector<Background>           m_Backgrounds;
    std::vector<Background>::iterator m_CurrentBackground;
};