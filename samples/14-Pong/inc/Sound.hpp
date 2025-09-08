#pragma once

#include <Audio/Waveform.hpp>

class Sound
{
public:
    Sound() = default;
    Sound( float frequency, float duration );

    void update( float deltaTime );

    void play();

    void stop();

    static Sound WallSound;                     // The ball bounces off the top or bottom wall.
    static Sound PaddleSound;                   // The ball bounces off a paddle.
    static Sound PointSound;                    // The ball bounces off a back wall (the opposite player receives a point).
    static void  updateAll( float deltaTime );  // Update static sounds.
    static void  stopAll();                     // Stop all sound effect.

private:
    float m_Frequency = 0.0f;
    float m_Duration  = 0.0f;
    float m_TotalTime = 0.0f;
    bool  m_IsPlaying = false;

    Audio::Waveform m_Waveform;
};
