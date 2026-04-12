#include <Timer.hpp>

#include <graphics/Image.hpp>
#include <graphics/Rasterizer.hpp>
#include <graphics/Window.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include <algorithm>
#include <cmath>
#include <format>
#include <string_view>

using namespace sr;

namespace
{
Font::Style styleFromIndex( int styleIndex )
{
    switch ( styleIndex )
    {
    case 1:
        return Font::Style::Bold;
    case 2:
        return Font::Style::Italic;
    case 3:
        return Font::Style::Bold | Font::Style::Italic;
    case 4:
        return Font::Style::Underline;
    case 5:
        return Font::Style::Strikethrough;
    default:
        return Font::Style::Normal;
    }
}

void drawTextWithEffects( Rasterizer& rasterizer, Font& font, std::string_view text, int x, int y, const Color& fillColor, const Color& outlineColor, bool drawOutline, int outlineSize,
                          bool drawShadow, const glm::ivec2& shadowOffset, const Color& shadowColor, bool drawGlow, int glowRadius, const Color& glowColor, bool drawWave,
                          float waveAmplitude, float wavePhase, int simulatedWeight )
{
    font.setOutline( drawOutline ? outlineSize : 0 );

    auto r = rasterizer;

    const int waveOffsetY = drawWave ? static_cast<int>( std::round( std::sin( wavePhase ) * waveAmplitude ) ) : 0;

    if ( drawGlow && glowRadius > 0 )
    {
        r.state.outlineColor = glowColor;
        r.state.color        = glowColor;

        for ( int oy = -glowRadius; oy <= glowRadius; ++oy )
        {
            for ( int ox = -glowRadius; ox <= glowRadius; ++ox )
            {
                if ( ox == 0 && oy == 0 )
                    continue;

                if ( ( ox * ox + oy * oy ) <= ( glowRadius * glowRadius ) )
                {
                    r.drawText( font, text, x + ox, y + oy + waveOffsetY );
                }
            }
        }
    }

    if ( drawShadow )
    {
        r.state.outlineColor = shadowColor;
        r.state.color        = shadowColor;
        r.drawText( font, text, x + shadowOffset.x, y + shadowOffset.y + waveOffsetY );
    }

    r.state.outlineColor = outlineColor;
    r.state.color        = fillColor;

    for ( int pass = 0; pass < std::max( 1, simulatedWeight ); ++pass )
    {
        const int offset = pass - ( simulatedWeight - 1 ) / 2;
        r.drawText( font, text, x + offset, y + waveOffsetY );
    }
}
}  // namespace

int main()
{
    constexpr int SCREEN_WIDTH  = 1280;
    constexpr int SCREEN_HEIGHT = 720;

    Window     window( "Fonts and Text", SCREEN_WIDTH, SCREEN_HEIGHT );
    Image      image { SCREEN_WIDTH, SCREEN_HEIGHT };
    Rasterizer rasterizer;
    Timer      timer;
    Text       fpsText { "FPS: 0" };

    Font defaultFont { 48.0f };
    Font loadedFont { "assets/fonts/ARCADE_N.ttf", 48.0f };

    float fontSize      = 48.0f;
    int   fontStyle     = 0;
    int   outlineSize   = 2;
    int   glowRadius    = 3;
    int   fontWeight    = 1;
    bool  showOutline   = true;
    bool  showShadow    = true;
    bool  showGlow      = true;
    bool  showWave      = true;
    float waveAmplitude = 6.0f;
    float waveSpeed     = 2.0f;
    float animationTime = 0.0f;
    glm::ivec2 shadowOffset { 4, 4 };

    glm::vec4 defaultFill  = { 1.0f, 0.95f, 0.20f, 1.0f };
    glm::vec4 loadedFill   = { 0.40f, 1.0f, 1.0f, 1.0f };
    glm::vec4 outlineColor = { 0.03f, 0.03f, 0.03f, 1.0f };
    glm::vec4 shadowColor  = { 0.0f, 0.0f, 0.0f, 0.45f };
    glm::vec4 glowColor    = { 0.30f, 0.70f, 1.0f, 0.18f };

    // Setup the rasterizer's render target state.
    rasterizer.state.colorTarget = &image;

    while ( window )
    {
        timer.tick();

        SDL_Event event;
        while ( window.pollEvent( event ) )
        {
            switch ( event.type )
            {
            case SDL_EVENT_KEY_DOWN:
                switch ( event.key.key )
                {
                case SDLK_ESCAPE:
                    window.destroy();
                    break;
                case SDLK_V:
                    window.toggleVSync();
                    break;
                case SDLK_F11:
                    window.toggleFullscreen();
                    break;
                case SDLK_RETURN:
                    if ( ( event.key.mod & SDL_KMOD_ALT ) != 0 )
                    {
                        window.toggleFullscreen();
                    }
                    break;
                }
                break;
            }
        }

        if ( ImGui::Begin( "Font Tweaks" ) )
        {
            static const char* styleItems[] = { "Normal", "Bold", "Italic", "Bold + Italic", "Underline", "Strikethrough" };

            ImGui::SliderFloat( "Font Size", &fontSize, 12.0f, 120.0f, "%.1f" );
            ImGui::Combo( "Font Style", &fontStyle, styleItems, IM_ARRAYSIZE( styleItems ) );
            ImGui::SliderInt( "Font Weight", &fontWeight, 1, 8, "Simulated x%d" );
            ImGui::Checkbox( "Outline", &showOutline );
            ImGui::SliderInt( "Outline Size", &outlineSize, 0, 8 );
            ImGui::Checkbox( "Drop Shadow", &showShadow );
            ImGui::SliderInt2( "Shadow Offset", &shadowOffset.x, -20, 20 );
            ImGui::Checkbox( "Outer Glow", &showGlow );
            ImGui::SliderInt( "Glow Radius", &glowRadius, 1, 12 );
            ImGui::Checkbox( "Wave Motion", &showWave );
            ImGui::SliderFloat( "Wave Amplitude", &waveAmplitude, 0.0f, 24.0f, "%.1f" );
            ImGui::SliderFloat( "Wave Speed", &waveSpeed, 0.1f, 8.0f, "%.1f" );

            ImGui::ColorEdit4( "Default Fill", glm::value_ptr( defaultFill ) );
            ImGui::ColorEdit4( "Loaded Fill", glm::value_ptr( loadedFill ) );
            ImGui::ColorEdit4( "Outline Color", glm::value_ptr( outlineColor ) );
            ImGui::ColorEdit4( "Shadow Color", glm::value_ptr( shadowColor ) );
            ImGui::ColorEdit4( "Glow Color", glm::value_ptr( glowColor ) );

            ImGui::Text( "Loaded Font: assets/fonts/ARCADE_N.ttf" );
            ImGui::Text( "Default Font Family: %s", defaultFont.getFamilyName().c_str() );
            ImGui::Text( "Loaded Font Family: %s", loadedFont.getFamilyName().c_str() );

            ImGui::End();
        }

        const Font::Style style = styleFromIndex( fontStyle );
        defaultFont.setSize( fontSize ).setStyle( style );
        loadedFont.setSize( fontSize ).setStyle( style );

        image.clear( Color::fromHTML( "#111827" ) );

        animationTime += static_cast<float>( timer.elapsedSeconds() );
        const float phase = animationTime * waveSpeed;

        drawTextWithEffects( rasterizer, defaultFont, "Default Font: The quick brown fox", 84, 180, Color::fromFloats( defaultFill ), Color::fromFloats( outlineColor ), showOutline,
                             outlineSize, showShadow, shadowOffset, Color::fromFloats( shadowColor ), showGlow, glowRadius, Color::fromFloats( glowColor ), showWave, waveAmplitude, phase,
                             fontWeight );

        drawTextWithEffects( rasterizer, loadedFont, "Loaded Font: 0123456789 !@#$%", 84, 320, Color::fromFloats( loadedFill ), Color::fromFloats( outlineColor ), showOutline, outlineSize,
                             showShadow, shadowOffset, Color::fromFloats( shadowColor ), showGlow, glowRadius, Color::fromFloats( glowColor ), showWave, waveAmplitude, phase + 0.75f,
                             fontWeight );

        if ( timer.totalSeconds() > 1.0 )
        {
            fpsText = std::format( "FPS: {:.0f}", timer.FPS() );
            timer.reset();
        }

        fpsText.setFillColor( Color::White );
        fpsText.setOutlineColor( Color::Black );
        rasterizer.drawText( fpsText, 12, 12 );

        window.clear( Color::Black );
        window.present( image );
    }

    return 0;
}
