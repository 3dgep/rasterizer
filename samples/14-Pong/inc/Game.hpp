#pragma once

#include "Menu.hpp"

#include <graphics/Rasterizer.hpp>
#include <graphics/Window.hpp>

class Game
{
public:
    enum class State
    {
        Menu,
        Game,
        GameOver
    };

    Game() = default;
    Game( int screenWidth, int screenHeight );

    void processEvents( const SDL_Event& event );

    void update( float deltaTime );

    void draw( sr::Rasterizer& rasterizer );

private:
    void updateMenu( float deltaTime );
    void drawMenu( sr::Rasterizer& rasterizer );

    State          m_GameState = State::Menu;
    Menu           m_MainMenu;
};