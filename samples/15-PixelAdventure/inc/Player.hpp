#pragma once

#include "Character.hpp"

#include <Audio/Sound.hpp>
#include <graphics/Rasterizer.hpp>
#include <math/Transform2D.hpp>

#include <vector>

class Player final
{
public:
    enum class State
    {
        Idle,
        Run,
        Jump,
        Hit,
        DoubleJump,
        Falling,
        LeftWallJump,
        RightWallJump,
        Dead,
    };

    explicit Player( const sr::math::Transform2D& transform = sr::math::Transform2D {} );

    /// <summary>
    /// Reset the character (and choose a different character).
    /// </summary>
    void reset();

    /// <summary>
    /// Update the player.
    /// </summary>
    /// <param name="deltaTime">The elapsed time (in seconds).</param>
    void update( float deltaTime ) noexcept;

    /// <summary>
    /// Draw the player to the image.
    /// </summary>
    /// <param name="rasterizer">The rasterizer to use to draw the player.</param>
    void draw( sr::graphics::Rasterizer& rasterizer ) const noexcept;

    /// <summary>
    /// Set the state of the player.
    /// </summary>
    /// <param name="newState">The new state to set the player to.</param>
    void setState( State newState );

    /// <summary>
    /// Check if the player is dead (this is used for level reloading).
    /// </summary>
    /// <returns></returns>
    bool isDead() const noexcept
    {
        return state == State::Dead;
    }

    /// <summary>
    /// Get the current player's state.
    /// </summary>
    /// <returns></returns>
    State getState() const noexcept
    {
        return state;
    }

    void setPosition( const glm::vec2& pos ) noexcept
    {
        transform.setPosition( pos );
    }

    const glm::vec2& getPosition() const noexcept
    {
        return transform.getPosition();
    }

    void setVelocity( const glm::vec2& vel ) noexcept
    {
        velocity = vel;
    }

    const glm::vec2& getVelocity() const noexcept
    {
        return velocity;
    }

    void setTransform( const sr::math::Transform2D& _transform )
    {
        transform = _transform;
    }

    const sr::math::Transform2D& getTransform() const noexcept
    {
        return transform;
    }

    sr::math::AABB getAABB() const noexcept
    {
        return transform * aabb;
    }

    sr::math::AABB getTopAABB() const noexcept
    {
        return transform * topAABB;
    }

    sr::math::AABB getBottomAABB() const noexcept
    {
        return transform * bottomAABB;
    }

    sr::math::AABB getLeftAABB() const noexcept
    {
        return transform * leftAABB;
    }

    sr::math::AABB getRightAABB() const noexcept
    {
        return transform * rightAABB;
    }

    void setCharacter( size_t characterId );

    size_t getNumCharacters() const noexcept
    {
        return characters.size();
    }

private:
    void startState( State oldState, State newState );
    void endState( State oldState, State newState );

    float doHorizontalMovement( float deltaTime );
    void  doIdle( float deltaTime );
    void  doRun( float deltaTime );
    void  doJump( float deltaTime );
    void  doHit( float deltaTime );
    void  doDoubleJump( float deltaTime );
    void  doFalling( float deltaTime );
    void  doWallJump( float deltaTime );

    // A list of possible character models.
    using CharacterList = std::vector<std::shared_ptr<Character>>;

    CharacterList              characters;
    std::shared_ptr<Character> currentCharacter;

    // The player's transform.
    sr::math::Transform2D transform;

    // The player's AABB.
    sr::math::AABB aabb;
    sr::math::AABB topAABB;
    sr::math::AABB bottomAABB;
    sr::math::AABB leftAABB;
    sr::math::AABB rightAABB;

    // Sound effects.
    Audio::Sound jumpSound;
    Audio::Sound hitSound;

    // Player's current state.
    State state = State::Idle;

    // How much dampening to apply.
    float xDampen = 50.0f;

    // How fast the player accelerates.
    static const float accel;
    // The maximum speed of the player.
    static const float maxSpeed;
    // Maximum jump height in pixels.
    static const float jumpHeight;
    // Jump time is the time (in seconds) to the apex of the jump.
    static const float jumpTime;

public:
    // Gravity and jump speed are computed to achieve the desired jump parabola.
    // Source: https://2dengine.com/?p=platformers#Jumping
    // Source: https://jobtalle.com/2d_platformer_physics.html
    static const float gravity;
    static const float jumpSpeed;
    // How fast the player jumps off the wall.
    static const float wallJumpSpeed;
    // How long after falling can the player still jump?
    static const float coyoteTime;
    // How long before landing the user can trigger a jump.
    static const float jumpBuffer;

private:
    // True if the player can double jump.
    bool canDoubleJump = true;

    // Allow jumping when falling for a short time.
    float fallTimer = 0.0f;
    float jumpTimer = 0.0f;

    // Player's current velocity.
    glm::vec2 velocity { 0 };
};