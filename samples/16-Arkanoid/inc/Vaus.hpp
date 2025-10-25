#pragma once

#include "Physics.hpp"

#include <graphics/Rasterizer.hpp>
#include <graphics/SpriteAnimation.hpp>

#include <math/AABB.hpp>
#include <math/Transform2D.hpp>

#include <glm/vec2.hpp>

class Ball;

class Vaus
{
public:
    enum class State
    {
        Wait,    // Wait for "ready" state.
        Appear,  // When vaus appears on the screen (after dying or at the beginning of the level).
        Default,
        ToLaser,  // Transition to laser mode.
        Laser,
        Enlarge,
        Dead,           // Vaus is dead.
        ExplodeStage1,  // First stage explosion.
        ExplodeStage2,  // Second stage explosion.
    };

    Vaus();

    Vaus( const std::shared_ptr<sr::graphics::SpriteSheet>& spriteSheet );

    void  setState( State newState );
    State getState() const noexcept;

    void update( float deltaTime );

    void draw( sr::graphics::Rasterizer& rasterizer );

    void             setPosition( const glm::vec2& pos );
    const glm::vec2& getPosition() const;

    // The extent is 1/2 the width.
    // The width of vaus is dependent on state.
    const glm::vec2& getAnchor() const noexcept;

    sr::math::AABB getAABB() const;

    /// <summary>
    /// Check for collision with the paddle.
    /// </summary>
    /// <param name="ball">The ball to check for collision with.</param>
    /// <returns>An optional HitInfo structure if a collision occurred.</returns>
    std::optional<Physics::HitInfo> collidesWith( const Ball& ball ) const;

private:
    // Framerate for all sprite animations.
    static inline const float FPS = 6.0f;

    void updateControls( float deltaTime );

    void beginState( State newState );
    void endState( State oldState );

    // Update functions for each state/mode.
    void doWait( float deltaTime );
    void doAppear( float deltaTime );
    void doDefault( float deltaTime );
    void doToLaser( float deltaTime );
    void doLaser( float deltaTime );
    void doEnlarge( float deltaTime );
    void doExplosion( float deltaTime );
    void doDead( float deltaTime );

    State state = State::Wait;

    sr::math::AABB   aabb;
    sr::math::AABB   enlargeAABB;

    sr::math::Transform2D transform;
    sr::math::Transform2D enlargeTransform;
    sr::math::Transform2D explosionTransform;

    sr::graphics::SpriteAnimation appearMode;
    sr::graphics::SpriteAnimation defaultMode;
    sr::graphics::SpriteAnimation enlargeMode;
    sr::graphics::SpriteAnimation toLaserMode;
    sr::graphics::SpriteAnimation laserMode;
    sr::graphics::SpriteAnimation explode1;
    sr::graphics::SpriteAnimation explode2;

    // Pixels per second.
    static inline const float speed = 300.0f;
};
