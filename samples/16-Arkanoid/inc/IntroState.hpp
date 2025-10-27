#include "Game.hpp"
#include "State.hpp"

#include <graphics/SpriteAnimation.hpp>

class IntroState : public State
{
public:
    IntroState( Game& game );

    void update( float deltaTime ) override;
    void draw( sr::graphics::Rasterizer& rasterizer ) override;

private:
    Game& game;

    sr::graphics::Image           starBackground;
    sr::graphics::SpriteAnimation shipAnimation;
    sr::graphics::SpriteAnimation shipDestroyedAnimation;
    float                         time1     = 0.0f;  // Used for text.
    float                         time2     = 0.0f;  // Used for animation timeline.
    int                           line      = 0;
    int                           character = 0;
    static constexpr float        cps       = 30.0f;  // Characters per second.
};