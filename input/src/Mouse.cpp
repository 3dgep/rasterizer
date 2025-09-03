#include <input/Mouse.hpp>

#include <SDL3/SDL_mouse.h>

using namespace sr::input;

MouseState Mouse::getState()
{
    MouseState state {};

    SDL_MouseButtonFlags buttons = SDL_GetMouseState( &state.x, &state.y );
    SDL_GetGlobalMouseState( &state.screenX, &state.screenY );

    state.leftButton   = ( buttons & SDL_BUTTON_LMASK ) != 0;
    state.middleButton = ( buttons & SDL_BUTTON_MMASK ) != 0;
    state.rightButton  = ( buttons & SDL_BUTTON_RMASK ) != 0;
    state.xButton1     = ( buttons & SDL_BUTTON_X1MASK ) != 0;
    state.xButton2     = ( buttons & SDL_BUTTON_X2MASK ) != 0;

    return state;
}