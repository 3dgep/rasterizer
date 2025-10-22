#pragma once

#include <graphics/Image.hpp>
#include <graphics/Rasterizer.hpp>
#include <graphics/Sprite.hpp>

#include <filesystem>

class Transition final
{
public:
    Transition()  = default;
    ~Transition() = default;

    Transition( const std::filesystem::path& fileName );

    Transition( const Transition& ) = default;
    Transition( Transition&& )      = default;

    Transition& operator=( const Transition& copy );
    Transition& operator=( Transition&& other ) noexcept;

    /// <summary>
    /// Set the ratio of the transition.
    /// 0 - beginning, 1 - end.
    /// </summary>
    /// <param name="ratio">The ratio [0...1] to set the transition to.</param>
    void setRatio( float ratio );

    /// <summary>
    /// Draw the transition to the image.
    /// </summary>
    /// <param name="rasterizer">The rasterizer to use to draw the transition effect.</param>
    void draw( sr::graphics::Rasterizer& rasterizer ) const;

private:
    std::shared_ptr<sr::graphics::Image> transition;
    sr::graphics::Sprite                 sprite;

    float                              time     = 0.0f;
    const float                        maxScale = 3.2f;
    std::vector<sr::math::Transform2D> transforms;
};