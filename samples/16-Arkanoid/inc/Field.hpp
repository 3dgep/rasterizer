#pragma once

#include <graphics/Rasterizer.hpp>
#include <graphics/SpriteAnimation.hpp>
#include <graphics/SpriteSheet.hpp>

#include <array>
#include <memory>

class Field
{
public:
    Field();

    explicit Field( std::shared_ptr<sr::graphics::SpriteSheet> fieldSprites );

    void setLevel( int level );

    void setLives( int lives );

    void update( float deltaTime );

    void draw( sr::graphics::Rasterizer& image );

private:
    static constexpr float FPS = 6.0f;

    // The current level.
    // The level determines which field and player ship sprite to show.
    int level = 0;
    // Number of player lives.
    int lives = 3;

    std::shared_ptr<sr::graphics::SpriteSheet> fieldSprites;
    std::array<sr::graphics::Sprite, 5>        shipSprites;
    sr::graphics::SpriteAnimation              exitAnim;
    sr::graphics::SpriteAnimation              leftEnemyGate;
    sr::graphics::SpriteAnimation              rightEnemyGate;
};