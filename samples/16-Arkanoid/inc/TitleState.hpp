#pragma once

#include "Button.hpp"
#include "State.hpp"

class Game;

class TitleState : public State
{
public:
    TitleState( Game& game );

    void update( float deltaTime ) override;
    void draw( sr::graphics::Rasterizer& rasterizer ) override;
    void processEvent( const SDL_Event& event ) override;

private:
    void onResized( const SDL_WindowEvent& args );
    void onMouseMoved( SDL_MouseMotionEvent& args );

    Game& game;

    int screenWidth;
    int screenHeight;

    sr::graphics::Sprite arkanoidSprite;
    sr::graphics::Sprite taitoSprite;

    // The game rectangle in the Window's coordinate frame.
    // Used for translating mouse coordinates.
    sr::math::RectI gameRect { 0 };
    // Translated mouse position.
    glm::ivec2 mousePos { 0 };
};
