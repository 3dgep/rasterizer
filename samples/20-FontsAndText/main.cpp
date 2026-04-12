#include "graphics/ResourceManager.hpp"

#include <Timer.hpp>

#include <graphics/Image.hpp>
#include <graphics/Rasterizer.hpp>
#include <graphics/Window.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include <cmath>
#include <format>
#include <string_view>

using namespace sr;

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

int main()
{
    constexpr int SCREEN_WIDTH  = 1280;
    constexpr int SCREEN_HEIGHT = 720;

    Window     window( "Fonts and Text", SCREEN_WIDTH, SCREEN_HEIGHT );
    Image      image { SCREEN_WIDTH, SCREEN_HEIGHT };
    Rasterizer rasterizer;
    Timer      timer;
    

    std::shared_ptr<Font> fpsFont     = ResourceManager::loadFont();
    fpsFont->setOutline( 1 );
    std::shared_ptr<Font> defaultFont = ResourceManager::loadFont( 48.0f );
    std::shared_ptr<Font> loadedFont  = ResourceManager::loadFont( "assets/fonts/arial.ttf", 48.0f );

    Text fpsText { fpsFont, "FPS: 0" };
    Text defaultText( defaultFont );
    Text loadedText( loadedFont );

    float      fontSize      = 48.0f;
    int        fontStyle     = 0;
    int        outlineSize   = 2;
    int        glowRadius    = 3;
    bool       showOutline   = true;
    bool       showShadow    = false;
    bool       showGlow      = false;
    bool       showWave      = false;
    float      waveAmplitude = 6.0f;
    float      waveSpeed     = 2.0f;
    float      animationTime = 0.0f;
    glm::ivec2 shadowOffset { 4, 4 };
    char       sampleText[256] { "The quick brown fox jumps over the lazy dog." };

    glm::vec4 fillColor    = { 0.9f, 0.9f, 0.9f, 1.0f };
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
            ImGui::ColorEdit4( "Fill Color", glm::value_ptr( fillColor ) );
            ImGui::Checkbox( "Outline", &showOutline );
            if ( showOutline )
            {
                ImGui::SliderInt( "Outline Size", &outlineSize, 0, 8 );
                ImGui::ColorEdit4( "Outline Color", glm::value_ptr( outlineColor ) );
            }
            ImGui::Checkbox( "Drop Shadow", &showShadow );
            if ( showShadow )
            {
                ImGui::SliderInt2( "Shadow Offset", &shadowOffset.x, -20, 20 );
                ImGui::ColorEdit4( "Shadow Color", glm::value_ptr( shadowColor ) );
            }
            ImGui::Checkbox( "Outer Glow", &showGlow );
            if ( showGlow )
            {
                ImGui::SliderInt( "Glow Radius", &glowRadius, 1, 12 );
                ImGui::ColorEdit4( "Glow Color", glm::value_ptr( glowColor ) );
            }
            ImGui::Checkbox( "Wave Motion", &showWave );
            if ( showWave )
            {
                ImGui::SliderFloat( "Wave Amplitude", &waveAmplitude, 0.0f, 24.0f, "%.1f" );
                ImGui::SliderFloat( "Wave Speed", &waveSpeed, 0.1f, 8.0f, "%.1f" );
            }
            ImGui::InputTextMultiline( "Example text", sampleText, 256 );
            ImGui::Separator();
            ImGui::Text( "Default Font Family: %s", defaultFont->getFamilyName().c_str() );
            ImGui::Text( "Loaded Font Family: %s", loadedFont->getFamilyName().c_str() );

            ImGui::End();
        }

        const Font::Style style = styleFromIndex( fontStyle );

        for ( auto& f: { defaultFont, loadedFont } )
        {
            f->setSize( fontSize ).setStyle( style ).setOutline( showOutline ? outlineSize : 0 );
        }

        for ( Text* t: { &defaultText, &loadedText } )
        {
            t->setFillColor( Color::fromFloats( fillColor ) )
                .setOutlineColor( Color::fromFloats( outlineColor ) )
                .setShadowColor( Color::fromFloats( shadowColor ) )
                .setGlowColor( Color::fromFloats( glowColor ) )
                .setShadowOffset( showShadow ? shadowOffset : glm::ivec2 { 0, 0 } )
                .setGlowRadius( showGlow ? glowRadius : 0 )
                .setWrapWidth( image.getWidth() - 84 )
                .setText( sampleText );
        }

        image.clear( Color::White );

        animationTime += static_cast<float>( timer.elapsedSeconds() );
        const float phase       = animationTime * waveSpeed;
        const int   waveOffsetY = showWave ? static_cast<int>( std::round( std::sin( phase ) * waveAmplitude ) ) : 0;

        rasterizer.drawText( defaultText, 84, 180 + waveOffsetY );
        auto textSize = defaultText.getOutlineSize();
        rasterizer.drawText( loadedText, 84, 180 + textSize.y + waveOffsetY );

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
