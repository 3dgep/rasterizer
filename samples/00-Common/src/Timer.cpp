#include <Timer.hpp>

#include <algorithm>
#include <thread>

using std::chrono::duration;

Timer::Timer( double fixedTimeStep ) noexcept
: fixedTimeStep( fixedTimeStep )
{
    reset();
}

void Timer::setFixedTimeStep( double seconds ) noexcept
{
    fixedTimeStep = seconds;
}

double Timer::getFixedTimeStep() const noexcept
{
    return fixedTimeStep;
}

void Timer::setMaxTimeStep( double seconds ) noexcept
{
    maxTimeStep = seconds;
}

double Timer::getMaxTimeStep() const noexcept
{
    return maxTimeStep;
}

void Timer::reset() noexcept
{
    t0         = clock::now();
    t1         = t0;
    beginFrame = t0;

    elapsedTime = 0.0;
    totalTime   = 0.0;
    ticks       = 0;
}

void Timer::tick( const UpdateFunc& f ) noexcept
{
    t1                           = clock::now();
    const duration<double> delta = t1 - t0;
    t0                           = t1;

    elapsedTime = delta.count();
    totalTime += elapsedTime;
    ++ticks;

    // Clamp the elapsed time to the maximum time step to prevent spiral of death.
    elapsedTime = std::min( elapsedTime, maxTimeStep );

    if ( fixedTimeStep > 0.0 )
    {
        accumulatedTime += elapsedTime;
        while ( accumulatedTime > fixedTimeStep )
        {
            if ( f )
                f( fixedTimeStep );

            accumulatedTime -= fixedTimeStep;
        }
    }
    else
    {
        if ( f )
            f( elapsedTime );
    }
}

double Timer::elapsedSeconds() const noexcept
{
    return elapsedTime;
}
double Timer::elapsedMilliseconds() const noexcept
{
    return elapsedTime * 1e3;
}

double Timer::elapsedMicroseconds() const noexcept
{
    return elapsedTime * 1e6;
}

double Timer::totalSeconds() const noexcept
{
    return totalTime;
}

double Timer::totalMilliseconds() const noexcept
{
    return totalTime * 1e3;
}
double Timer::totalMicroseconds() const noexcept
{
    return totalTime * 1e6;
}

double Timer::FPS() const noexcept
{
    return static_cast<double>( ticks ) / totalTime;
}

void Timer::limitFPS( int fps ) const noexcept
{
    limitFPS( 1.0 / static_cast<double>( std::max( 1, fps ) ) );
}

void Timer::limitFPS( double seconds ) const noexcept
{
    limitFPS( duration<double>( std::max( 0.0, seconds ) ) );
}

void Timer::limitFPS( const clock::duration& duration ) const noexcept
{
    const auto endTime = beginFrame + duration;

#if 1
    // This uses less CPU power than a busy loop, but it's less accurate.
    std::this_thread::sleep_until( endTime );
#else
    // This busy loop uses more CPU power than this_thread::sleep_until, but it's more accurate.
    while ( high_resolution_clock::now() < endTime )
        std::this_thread::yield();
#endif

    beginFrame = endTime;
}
