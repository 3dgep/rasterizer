#pragma once

#include <graphics/Rasterizer.hpp>
#include <graphics/SpriteAnimation.hpp>
#include <graphics/SpriteSheet.hpp>

#include <math/AABB.hpp>
#include <math/Transform2D.hpp>

#include <span>

class Bullet
{
public:
    enum class State
    {
        None,   // Bullet should not be drawn updated, or perform collisions.
        Fired,  // Bullet is fired.
        Hit,    // Bullet hit something.
    };

    Bullet() = default;

    Bullet( std::shared_ptr<sr::graphics::SpriteSheet> spriteSheet, std::span<const int> frames );

    void update( float deltaTime ) noexcept;
    void draw( sr::graphics::Rasterizer& rasterizer ) const noexcept;

    /// <summary>
    /// Fire the bullet from a particular position.
    /// </summary>
    /// <param name="pos">The position to fire the bullet from.</param>
    void fire( const glm::vec2& pos );

    /// <summary>
    /// Invoke when the Bullet hits something (either a brick or an enemy)
    /// </summary>
    void hit() noexcept;

    void             setPosition( const glm::vec2& pos ) noexcept;
    const glm::vec2& getPosition() const noexcept;

    sr::math::AABB getAABB() const noexcept;

    void  setState( State state ) noexcept;
    State getState() const noexcept;

private:
    static inline float     FPS   = 12.0f;
    static inline glm::vec2 SPEED = { 0.0f, 300.0f };  // How fast the bullet moves up the screen.

    void doFired( float deltaTime );
    void doHit( float deltaTime );

    sr::graphics::SpriteAnimation spriteAnim;
    sr::math::AABB                aabb;
    sr::math::Transform2D         transform;
    State                         state = State::None;
};