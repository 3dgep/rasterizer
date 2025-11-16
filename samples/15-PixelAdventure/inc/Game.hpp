#pragma once

#include "Background.hpp"
#include "Button.hpp"
#include "Level.hpp"
#include "Transition.hpp"

#include <Timer.hpp>

#include <graphics/Font.hpp>
#include <graphics/Image.hpp>

#include <math/Rect.hpp>

#include <LDtkLoader/Project.hpp>

#include <SDL3/SDL_events.h>

class Game
{
public:
    Game( uint32_t screenWidth, uint32_t screenHeight );
    ~Game() = default;

    Game( const Game& )            = delete;
    Game( Game&& )                 = delete;
    Game& operator=( const Game& ) = delete;
    Game& operator=( Game&& )      = delete;

    void update();

    const sr::graphics::Image& getImage() const noexcept
    {
        return image;
    }

    void processEvent( const SDL_Event& event );

    void onMouseMoved( SDL_MouseMotionEvent& args );
    void onResized( SDL_WindowEvent& args );

    // Button handlers.
    void onPreviousClicked();
    void onNextClicked();
    void onRestartClicked();

    void loadLevel( size_t levelId, size_t characterId );

private:
    void drawFPS();

    enum class TransitionState
    {
        None,
        In,
        Out,
    };

    ldtk::Project project;

    sr::graphics::Image      image;
    sr::graphics::Rasterizer rasterizer;
    Timer                    timer;
    bool                     paused = false;

    // Maximum tick time for physics.
    const float physicsTick = 1.0f / 60.0f;

    // The game rectangle in the Window's coordinate frame.
    // Used for translating mouse coordinates.
    sr::math::RectI gameRect;

    // Translated mouse position.
    glm::vec2 mousePos;

    // Fonts.
    sr::graphics::Font arial20;
    sr::graphics::Font arial24;

    // Backgrounds
    using BackgroundList = std::vector<Background>;
    BackgroundList           backgrounds;
    BackgroundList::iterator currentBackground;

    // Level transition effect.
    Transition      transition;
    const float     transitionDuration = 0.3f;
    float           transitionTime     = transitionDuration;
    TransitionState transitionState    = TransitionState::None;

    // Buttons
    Button previousButton;
    Button nextButton;
    Button restartButton;

    // Levels
    Level  currentLevel;
    size_t currentLevelId = 0u;
    // Which level to play next.
    size_t nextLevelId = 0u;

    // Player character.
    size_t currentCharacterId = 0u;
};
