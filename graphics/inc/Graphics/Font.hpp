#pragma once

#include "glm/vec2.hpp"

#include <filesystem>

struct TTF_Font;

namespace sr
{
inline namespace graphics
{
class Font
{
public:
    enum class HorizontalAlignment
    {
        Invalid,  // Font is invalid.
        Left,     // Left aligned.
        Center,   // Center aligned.
        Right     // Right aligned.
    };

    enum class Direction
    {
        Invalid,  // Invalid font.
        LTR,      // Left to right.
        RTL,      // Right to left.
        TTB,      // Top to bottom.
        BTT,      // Bottom to top.
    };

    enum class Style
    {
        Normal        = 0x0,  // No special style.
        Bold          = 0x1,  // Bold style.
        Italic        = 0x2,  // Italic style.
        Underline     = 0x4,  // Underlined text.
        Strikethrough = 0x8   // Strikethrough text.
    };

    enum class Hinting
    {
        Invalid,        // Invalid font.
        Normal,         // Normal hinting applies standard grid fitting.
        Light,          // Light hinting applies subtle adjustments to improve rendering.
        Mono,           // Monochrome hinting adjusts the font for better rendering at lower resolutions.
        None,           // No hinting. The font is rendered without any grid-fitting.
        LightSubpixel,  // Light hinting with subpixel rendering for more precise font edges.
    };

    enum class Weight
    {
        Thin       = 100,
        ExtraLight = 200,
        Light      = 300,
        Normal     = 400,
        Medium     = 500,
        SemiBold   = 600,
        Bold       = 700,
        ExtraBold  = 800,
        Black      = 900,
        ExtraBlack = 950
    };

    Font( float size = 12.0f );
    Font( const std::filesystem::path& fontFile, float size = 12.0f );
    Font( const Font& other );
    Font( Font&& other ) noexcept;

    Font& operator=( const Font& other );
    Font& operator=( Font&& other ) noexcept;

    ~Font();

    /// <summary>
    /// Query the offset from the baseline to the top of a font.
    /// </summary>
    /// <remarks>This is a positive value, relative to the baseline.</ remarks>
    /// <returns>The font's ascent.</returns>
    int getAscent() const;

    /// <summary>
    /// Query the offset from the baseline to the bottom of a font.
    /// </summary>
    /// <remarks>This is a negative value, relative to the baseline.</ remarks>
    /// <returns>The font's decent.</returns>
    int getDecent() const;

    /// <summary>
    /// Sets the text direction for the font.
    /// </summary>
    /// <param name="direction">The text direction to set (e.g., left-to-right or right-to-left).</param>
    /// <returns>A reference to the modified Font object.</returns>
    Font& setDirection( Direction direction );

    /// <summary>
    /// Retrieves the current direction.
    /// </summary>
    /// <returns>The current font direction.</returns>
    Direction getDirection() const;

    /// <summary>
    /// Get the font's horizontal dots per inch (DPI).
    /// </summary>
    /// <returns>The horizontal DPI value as an integer.</returns>
    int getHorizontalDPI() const;

    /// <summary>
    /// Get the font's vertical dots per inch (DPI).
    /// </summary>
    /// <returns>The vertical DPI value as an integer.</returns>
    int getVerticalDPI() const;

    /// <summary>
    /// Sets the size of an object in device-independent units based on horizontal and vertical DPI (dots per inch).
    /// </summary>
    /// <param name="size">The point size value to be set, in device-independent units.</param>
    /// <param name="hdpi">The horizontal DPI (dots per inch) of the display.</param>
    /// <param name="vdpi">The vertical DPI (dots per inch) of the display.</param>
    /// <returns>A reference to the modified Font object.</returns>
    Font& setSizeDPI( float size, int hdpi, int vdpi );

    /// <summary>
    /// Query the font's family name.
    /// </summary>
    /// <remarks>This string is dictated by the contents of the font file.</remarks>
    /// <returns>The font's family name, or an empty string if the font is invalid.</returns>
    std::string getFamilyName() const;

    /// <summary>
    /// Get the font's height.
    /// </summary>
    /// <returns>The font's height.</returns>
    int getHeight() const;

    /// <summary>
    /// Get the font point size.
    /// </summary>
    /// <returns>Returns the size of the font, or 0.0f on failure.</returns>
    float getSize() const;

    /// <summary>
    /// Sets the font point size.
    /// </summary>
    /// <param name="size">The point size.</param>
    /// <returns>A reference to the modified font.</returns>
    Font& setSize( float size );

    /// <summary>
    /// Query the font's FreeType hinter setting.
    /// </summary>
    /// <returns>The font's current hinter value.</returns>
    Hinting getHinting() const;

    /// <summary>
    /// Sets the font's hinting setting.
    /// </summary>
    /// <param name="hinting">The hinting setting to apply to the font.</param>
    /// <returns>A reference to the modified Font object.</returns>
    Font& setHinting( Hinting hinting );

    /// <summary>
    /// Checks whether kerning is enabled.
    /// </summary>
    /// <returns>true if kerning is enabled; otherwise, false.</returns>
    bool isKerning() const;

    /// <summary>
    /// Enables or disables kerning for the font.
    /// </summary>
    /// <param name="enabled">If true, kerning is enabled; if false, kerning is disabled.</param>
    /// <returns>A reference to the Font object with the updated kerning setting.</returns>
    Font& setKerning( bool enabled );

    /// <summary>
    /// Get the spacing between lines of text.
    /// </summary>
    /// <returns>The font's recommended line spacing.</returns>
    int getLineSpacing() const;

    /// <summary>
    /// Set the spacing between lines of text.
    /// </summary>
    /// <param name="spacing">The line spacing for the font.</param>
    /// <returns>A reference to the modified font object.</returns>
    Font& setLineSpacing( int spacing );

    /// <summary>
    /// Get the additional character spacing in pixels to be applied between any two rendered characters.
    /// </summary>
    /// <remarks>This defaults to 0 if it hasn't been set.</remarks>
    /// <returns>The character spacing in pixels</returns>
    int getCharSpacing() const;

    /// <summary>
    /// Set additional space in pixels to be applied between any two rendered characters.
    /// </summary>
    /// <remarks>
    /// The spacing value is applied uniformly after each character, in addition to the normal glyph's advance.
    /// Spacing may be a negative value, in which case it will reduce the distance instead.
    /// </ remarks>
    /// <returns>A reference to the font object with the updated character spacing.</returns>
    Font& setCharSpacing( int spacing );

    /// <summary>
    /// Query if Signed Distance Field (SDF) rendering is enabled.
    /// </summary>
    /// <returns>True if SDF rendering is enabled, false otherwise.</returns>
    bool isSDF() const;

    /// <summary>
    /// Enables or disables Signed Distance Field (SDF) rendering.
    /// </summary>
    /// <param name="enabled">If true, enables SDF rendering; if false, disables it.</param>
    /// <returns>A reference to the modified Font object.</returns>
    Font& setSDF( bool enabled );

    /// <summary>
    /// Checks whether the object is scalable.
    /// </summary>
    /// <returns>true if the object is scalable; otherwise, false.</returns>
    bool isScalable() const;

    /// <summary>
    /// Check if this is a fixed-width font.
    /// </summary>
    /// <remarks>
    /// A "fixed-width" font means all glyphs are the same width across;
    /// a lowercase 'i' will be the same size across as a capital 'W', for example.
    /// This is common for terminals and text editors, and other apps that treat text as a grid.
    /// Most other things (WYSIWYG word processors, web pages, etc) are more likely
    /// to not be fixed-width in most cases.
    /// </remarks>
    /// <returns>true if the font is fixed-width, false otherwise.</returns>
    bool isFixedWidth() const;

    /// <summary>
    /// Query the font's current style.
    /// </summary>
    /// <remarks>The font styles are a set of bit flags OR'd together.</remarks>
    /// <returns>The current font style.</returns>
    Style getStyle() const;

    /// <summary>
    /// Sets the font's style.
    /// </summary>
    /// <param name="style">A bit field of styles to enable OR'd together.</param>
    /// <returns>A reference to the modified font.</returns>
    Font& setStyle( Style style );

    /// <summary>
    /// Get the font's outline.
    /// </summary>
    /// <returns>Returns the font's current outline value.</returns>
    int getOutline() const;

    /// <summary>
    /// Enables or configures the outline for the font.
    /// </summary>
    /// <returns>A reference to the Font object with the outline set.</returns>
    Font& setOutline(int outline);

    /// <summary>
    /// Get the font's weight.
    /// </summary>
    /// <returns>The font's current weight.</returns>
    Weight getWeight() const;

    /// <summary>
    /// Get the font's wrap alignment.
    /// </summary>
    /// <returns>The font's current wrap alignment.</returns>
    HorizontalAlignment getWrapAlignment() const;

    /// <summary>
    /// Sets font's wrap alignment.
    /// </summary>
    /// <param name="alignment">The desired horizontal alignment for text wrapping.</param>
    /// <returns>A reference to the modified Font object.</returns>
    Font& setWrapAlignment( HorizontalAlignment alignment );

    /// <summary>
    /// Calculate the dimensions of a rendered (wrapped) string.
    /// </summary>
    /// <param name="text">A UTF-8 encoded string to calculate the size for.</param>
    /// <param name="wrapWidth">The maximum wrap width in pixels (or 0 to wrap on newline characters). Default is 0 to wrap on newline characters.</param>
    /// <returns>The width and height of the rendered string (in pixels).</returns>
    glm::ivec2 getStringSize( std::string_view text, int wrapWidth = 0 ) const;

private:
    friend class Text;
    Font( TTF_Font* font );
    TTF_Font* m_Font = nullptr;
};
}  // namespace graphics
}  // namespace sr

#include <math/bitmask_operators.hpp>

template<>
struct enable_bitmask_operators<sr::Font::Style>
{
    static constexpr bool enable = true;
};