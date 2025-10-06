#pragma once

#include "StateBase.hpp"
#include "GameState.hpp"

#include <graphics/Rasterizer.hpp>

class MenuState : public StateBase
{
public:
    MenuState() = default;

    MenuState( int screenWidth, int screenHeight );

    void beginState() override;

    State* update( float deltaTime ) override;

    void draw(sr::graphics::Rasterizer& rasterizer) override;

private:
    GameState::AIDifficulty m_SelectedAIDifficulty = GameState::AIDifficulty::Medium;
};