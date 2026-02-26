#pragma once

#include <chrono>
#include <functional>

class Timer
{
public:
    using clock      = std::chrono::steady_clock;
    using UpdateFunc = std::function<void( double )>;

    /// <summary>
    /// Construct a Timer with an optional fixed time step. If a fixed time step is provided,
    /// then the timer will accumulate elapsed time and call the update function with a fixed time step.
    /// A fixed time step of zero (the default) disables the fixed time step behavior and allows the timer to use the actual elapsed time between ticks.
    ///</summary>
    Timer( double fixedTimeStep = 0.0 ) noexcept;

    /// <summary>
    /// Set a fixed time step for the timer. If a fixed time step is set,
    /// the timer will accumulate elapsed time and call the update function
    /// with a fixed time step until the accumulated time is less than the fixed time step.
    /// This can be used to implement a fixed update loop for game logic or physics updates.
    /// Setting the fixed time step to zero disables the fixed time step behavior and allows
    /// the timer to use the actual elapsed time between ticks.
    /// </summary>
    void setFixedTimeStep( double seconds ) noexcept;

    double getFixedTimeStep() const noexcept;

    /// <summary>
    /// Sets the maximum time step to prevent spiral of death when using a fixed time step.
    /// This is a safety measure to prevent the timer from trying to catch up too much if the application was paused or running very slowly.
    /// </summary>
    /// <param name="seconds">The maximum time step in seconds.</param>
    void   setMaxTimeStep( double seconds ) noexcept;
    double getMaxTimeStep() const noexcept;

    /// <summary>
    /// Tick the timer. Pass an optional update function that will be called with the delta time (in seconds).
    /// If a fixed time step is set, the update function will be called with a fixed time step until the accumulated time is less than the fixed time step.
    /// If no fixed time step is set, the update function will be called once with the actual elapsed time since the last tick.
    /// </summary>
    void tick( const UpdateFunc& f = {} ) noexcept;

    /// <summary>
    /// Reset the timer. This will set the elapsed and total time to zero, reset the tick count,
    /// and set the internal time points to the current time.
    /// </summary>
    void reset() noexcept;

    double elapsedSeconds() const noexcept;
    double elapsedMilliseconds() const noexcept;
    double elapsedMicroseconds() const noexcept;

    double totalSeconds() const noexcept;
    double totalMilliseconds() const noexcept;
    double totalMicroseconds() const noexcept;

    /// <summary>
    /// Get the Frames Per Second (FPS) since the timer was last reset.
    /// </summary>
    /// <returns>The frame-rate since the timer was last reset.</returns>
    double FPS() const noexcept;

    void limitFPS( int fps ) const noexcept;
    void limitFPS( double seconds ) const noexcept;
    void limitFPS( const clock::duration& duration ) const noexcept;

    template<class Rep, class Period>
    constexpr void limitFPS( const std::chrono::duration<Rep, Period>& duration ) const noexcept
    {
        limitFPS( std::chrono::duration_cast<clock::duration>( duration ) );
    }

private:
    clock::time_point         t0, t1;
    mutable clock::time_point beginFrame;

    // The time elapsed since the last tick in seconds.
    double elapsedTime = 0.0;
    // The total time elapsed since the timer was last reset in seconds.
    double totalTime = 0.0;
    // The fixed time step to use for limiting FPS. If zero, no fixed time step is used.
    double fixedTimeStep = 0.0;
    // The accumulated time since the last update when using a fixed time step.
    double accumulatedTime = 0.0;
    // The maximum time step to prevent spiral of death when using a fixed time step.
    // This is a safety measure to prevent the timer from trying to catch up too much if the application was paused or running very slowly.
    // By default, don't limit the time step, but users can set this to a reasonable value (e.g., 0.25 seconds) to prevent issues.
    double maxTimeStep = std::numeric_limits<double>::max();

    uint64_t ticks = 0;
};