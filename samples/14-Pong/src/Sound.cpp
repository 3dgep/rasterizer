#include "Audio/Waveform.hpp"

#include <Sound.hpp>

// Sound frequency and durations
// Source: https://stelioskanitsakis.medium.com/an-audio-comparison-between-the-sounds-of-ataris-pong-and-the-silence-of-magnavox-odyssey-s-83e6fac56653
Sound Sound::WallSound { 226.0f, 0.016f };
Sound Sound::PaddleSound { 459.0f, 0.096f };
Sound Sound::PointSound { 490.0f, 0.257f };

Sound::Sound( float frequency, float duration )
: m_Frequency( frequency )
, m_Duration( duration )
, m_Waveform( Audio::Waveform::Type::Triangle, 1.0f, m_Frequency )
{
}

void Sound::update( float deltaTime )
{
    if (m_IsPlaying)
    {
        m_TotalTime += deltaTime;
        if ( m_TotalTime > m_Duration )
            stop();
    }
}

void Sound::play()
{
    m_Waveform.start();
    m_TotalTime = 0.0f;
    m_IsPlaying = true;
}

void Sound::stop()
{
    m_Waveform.stop();
    m_IsPlaying = false;
}

void Sound::updateAll( float deltaTime )
{
    WallSound.update( deltaTime );
    PaddleSound.update( deltaTime );
    PointSound.update( deltaTime );
}

void Sound::stopAll()
{
    WallSound.stop();
    PaddleSound.stop();
    PointSound.stop();
}