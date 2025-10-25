#pragma once

#include <graphics/Rasterizer.hpp>
#include <graphics/Sprite.hpp>

#include <math/Camera2D.hpp>
#include <math/Circle.hpp>

#include <glm/vec2.hpp>

class Ball
{
public:
    Ball();

    explicit Ball( const glm::vec2& position );

    void update( float deltaTime );

    void draw( sr::graphics::Rasterizer& rasterizer );

    void             setPosition( const glm::vec2& pos );
    const glm::vec2& getPosition() const;

    void             setVelocity( const glm::vec2& vel );
    const glm::vec2& getVelocity() const;

    void                    setCircle( const sr::math::Circle& circle );
    const sr::math::Circle& getCircle() const;

private:
    sr::math::Transform2D transform;
    sr::math::Circle      circle;
    glm::vec2             velocity { 0 };

    sr::graphics::Sprite sprite;

    static inline float radius = 2.0f;
};