#pragma once

#include <graphics/Rasterizer.hpp>

#include <SDL3/SDL_events.h>

class State
{
public:
    virtual void update( float deltaTime ) {}
    virtual void draw( sr::graphics::Rasterizer& image ) {}
    virtual void processEvent( const SDL_Event& event ) {}

    virtual ~State() = default;

protected:
    State()                          = default;
    State( const State& )            = default;
    State( State&& )                 = default;
    State& operator=( const State& ) = default;
    State& operator=( State&& )      = default;
};
