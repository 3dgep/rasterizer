#pragma once

#include "State.hpp"

#include <graphics/Rasterizer.hpp>
#include <graphics/Window.hpp>

class Game
{
public:
    Game() = default;
    Game( int screenWidth, int screenHeight );
    ~Game();

    void update( float deltaTime );

    void draw( sr::Rasterizer& rasterizer );

private:
    State* m_GameState = nullptr;
};