#pragma once

#include <graphics/Rasterizer.hpp>

#include <glm/vec2.hpp>

#include <optional>

struct HitInfo
{
    glm::vec2 normal;
    float     depth;
};

class Ball
{
public:
    Ball() = default;                                               // Default constructor.
    Ball( const glm::vec2& pos, float radius, float mass = 1.0f );  // Parameterized constructor.

    const glm::vec2& getPosition() const;
    void             setPosition( const glm::vec2& pos );  // non-const!

    const glm::vec2& getVelocity() const;
    void             setVelocity( const glm::vec2& vel );

    const glm::vec2& getAcceleration() const;
    void             setAcceleration( const glm::vec2& acc );

    void addAcceleration( const glm::vec2& acc );
    void addForce( const glm::vec2& force );

    float getRadius() const;
    void  setRadius( float radius );

    float getMass() const;
    void  setMass( float mass );

    std::optional<HitInfo> checkCollision( const Ball& other ) const;

    // Update the ball
    // deltaTime: Elapsed time in seconds.
    void update( float deltaTime );

    // Draw the ball to the screen.
    void draw( const sr::graphics::Rasterizer& rasterizer ) const;

private:
    sr::math::Circle circle;              // The collision shape for the ball.
    glm::vec2        velocity { 0 };      // Initial velocity both direction and magnitude (speed).
    glm::vec2        acceleration { 0 };  // Acceleration of the ball.
    float            mass { 1.0f };       // Mass of the ball
};
