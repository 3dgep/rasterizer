#pragma once

#include "Color.hpp"
#include "Font.hpp"
#include "Image.hpp"

#include <iostream>

struct SDL_ttf_context;
struct TTF_Text;
struct SDL_Surface;

namespace sr
{
inline namespace graphics
{
class Text
{
public:
    enum class Direction
    {
        Invalid,  // Text is not initialized yet.
        LTR,      // Left to right.
        RTL,      // Right to left.
        TTB,      // Top to bottom.
        BTT,      // Bottom to top.
    };

    /// <summary>
    /// Default constructor. Creates an uninitialized Text object.
    /// </summary>
    Text() = default;

    /// <summary>
    /// Constructs a Text object with the specified string and colors using the default font.
    /// </summary>
    /// <param name="text">The UTF-8 encoded text string.</param>
    /// <param name="fillColor">The fill color of the text.</param>
    /// <param name="outlineColor">The outline color of the text.</param>
    explicit Text( std::string_view text, const Color& fillColor = Color::White, const Color& outlineColor = Color::Black );

    /// <summary>
    /// Constructs a Text object with the specified font, string, and colors.
    /// </summary>
    /// <param name="font">The font to use for rendering the text.</param>
    /// <param name="text">The UTF-8 encoded text string.</param>
    /// <param name="fillColor">The fill color of the text.</param>
    /// <param name="outlineColor">The outline color of the text.</param>
    explicit Text( std::shared_ptr<const Font> font, std::string_view text = {}, const Color& fillColor = Color::White, const Color& outlineColor = Color::Black );

    /// <summary>
    /// Copy constructor.
    /// </summary>
    Text( const Text& );

    /// <summary>
    /// Move constructor.
    /// </summary>
    Text( Text&& ) noexcept;

    /// <summary>
    /// Destructor.
    /// </summary>
    ~Text();

    /// <summary>
    /// Copy assignment operator.
    /// </summary>
    /// <returns>A reference to the modified Text object.</returns>
    Text& operator=( const Text& );

    /// <summary>
    /// Move assignment operator.
    /// </summary>
    /// <returns>A reference to the modified Text object.</returns>
    Text& operator=( Text&& ) noexcept;

    /// <summary>
    /// Set the text string.
    /// </summary>
    /// <param name="string">The string to set the text to.</param>
    /// <returns>A reference to the Text object.</returns>
    Text& operator=( std::string_view string );

    /// <summary>
    /// Append UTF-8 encoded text to the Text object.
    /// </summary>
    /// <param name="string">The string to append to the Text object.</param>
    /// <returns>A reference to the modified Text object.</returns>
    Text& operator+=( std::string_view string );

    /// <summary>
    /// Set the text string.
    /// </summary>
    /// <param name="string">The string to set the text to.</param>
    /// <returns>A reference to the Text object.</returns>
    Text& setText( std::string_view string );

    /// <summary>
    /// Returns a read-only view of the text.
    /// </summary>
    /// <returns>A std::string_view representing the text.</returns>
    std::string_view getText() const;

    /// <summary>
    /// Append UTF-8 encoded text to the Text object.
    /// </summary>
    /// <param name="string">The string to append to the Text object.</param>
    /// <returns>A reference to the modified Text object.</returns>
    Text& appendString( std::string_view string );

    /// <summary>
    /// Get the color of the text object.
    /// </summary>
    /// <returns>The color of the text object.</returns>
    Color getFillColor() const;

    /// <summary>
    /// Sets the fill color of the text object.
    /// </summary>
    /// <param name="color">The color to apply to the text.</param>
    /// <returns>A reference to the modified Text object.</returns>
    Text& setFillColor( const Color& color );

    /// <summary>
    /// Gets the outline color of the text.
    /// </summary>
    /// <returns>The outline color.</returns>
    Color getOutlineColor() const;

    /// <summary>
    /// Sets the outline color of the text.
    /// </summary>
    /// <param name="color">The outline color to set.</param>
    /// <returns>A reference to the text object for method chaining.</returns>
    Text& setOutlineColor( const Color& color );

    /// <summary>
    /// Gets the shadow color.
    /// </summary>
    /// <returns>The shadow color.</returns>
    Color getShadowColor() const;

    /// <summary>
    /// Sets the shadow color of the text.
    /// </summary>
    /// <param name="color">The shadow color to set.</param>
    /// <returns>A reference to the modified Text object.</returns>
    Text& setShadowColor( const Color& color );

    /// <summary>
    /// Gets the shadow offset of the text.
    /// </summary>
    /// <returns>The shadow offset as a 2D integer vector (in pixels).</returns>
    const glm::ivec2& getShadowOffset() const;

    /// <summary>
    /// Sets the shadow offset of the text.
    /// </summary>
    /// <param name="offset">The shadow offset to set (in pixels).</param>
    /// <returns>A reference to the modified Text object.</returns>
    Text& setShadowOffset( const glm::ivec2& offset );

    /// <summary>
    /// Gets the glow color of the text.
    /// </summary>
    /// <returns>The glow color.</returns>
    Color getGlowColor() const;

    /// <summary>
    /// Sets the glow color of the text.
    /// </summary>
    /// <param name="color">The glow color to set.</param>
    /// <returns>A reference to the modified Text object.</returns>
    Text& setGlowColor( const Color& color );

    /// <summary>
    /// Gets the glow radius of the text.
    /// </summary>
    /// <returns>The glow radius (in pixels).</returns>
    int getGlowRadius() const;

    /// <summary>
    /// Sets the glow radius of the text.
    /// </summary>
    /// <param name="radius">The glow radius to set (in pixels).</param>
    /// <returns>A reference to the modified Text object.</returns>
    Text& setGlowRadius( int radius );

    /// <summary>
    /// Get the direction to be used for text shaping a text object.
    /// </summary>
    /// <returns>The direction the text will flow when rendering.</returns>
    Direction getDirection() const;

    /// <summary>
    /// Set the direction to be used for text shaping a text object.
    /// </summary>
    /// <param name="direction">The direction for the text to flow.</param>
    /// <returns>A reference to the modified text object.</returns>
    Text& setDirection( Direction direction );

    /// <summary>
    /// Get the font used by the text object.
    /// </summary>
    /// <returns>The font used by the text object.</returns>
    std::shared_ptr<const Font> getFont() const;

    /// <summary>
    /// Sets the font used by the text object.
    /// </summary>
    /// <param name="font">A reference to the Font object to be used for rendering the text.</param>
    /// <returns>A reference to the modified Text object.</returns>
    Text& setFont( std::shared_ptr<const Font> font );

    /// <summary>
    /// Get the position of the text object.
    /// </summary>
    /// <returns>The top-left corner of the Text object (in pixels).</returns>
    glm::ivec2 getPosition() const;

    /// <summary>
    /// Sets the position of the Text object.
    /// </summary>
    /// <param name="pos">The new top-left position of the Text object.</param>
    /// <returns>A reference to the modified Text object.</returns>
    Text& setPosition( const glm::ivec2& pos );

    /// <summary>
    /// Get the size of the text (in pixels).
    /// </summary>
    /// <returns>The width & height of the rendered Text object (in pixels).</returns>
    glm::ivec2 getFillSize() const;

    /// <summary>
    /// Gets the size of the text with the outline stroke.
    /// </summary>
    /// <returns>A 2D integer vector representing the size of the text with the outline stroke (in Pixels).</returns>
    glm::ivec2 getOutlineSize() const;

    /// <summary>
    /// Gets the width of the filled text (in pixels).
    /// </summary>
    /// <returns>The width of the filled text (in pixels).</returns>
    int getFillWidth() const;

    /// <summary>
    /// Gets the width of the text with the outline stroke (in pixels).
    /// </summary>
    /// <returns>The width of the outlined text (in pixels).</returns>
    int getOutlineWidth() const;

    /// <summary>
    /// Gets the height of the filled text (in pixels).
    /// </summary>
    /// <returns>The height of the filled text (in pixels).</returns>
    int getFillHeight() const;

    /// <summary>
    /// Gets the height of the text with the outline stroke (in pixels).
    /// </summary>
    /// <returns>The height of the outlined text (in pixels).</returns>
    int getOutlineHeight() const;

    /// <summary>
    /// Get the wrap width of the Text object.
    /// </summary>
    /// <returns>The wrap width of the Text object, or 0 if the text is wrapped on newline characters.</returns>
    int getWrapWidth() const;

    /// <summary>
    /// Set the wrap width of the Text object.
    /// </summary>
    /// <param name="width">The wrap width (in pixels) of the Text object. Set to 0 to wrap on newline characters.</param>
    /// <returns>A reference to the modified Text object.</returns>
    Text& setWrapWidth( int width );

    /// <summary>
    /// Draws the Text at the specified position on the image.
    /// </summary>
    /// <param name="image">The image to draw on.</param>
    /// <param name="x">The x-coordinate of the position.</param>
    /// <param name="y">The y-coordinate of the position.</param>
    void draw( Image& image, int x, int y ) const;

private:
    // Intentionally unscoped.
    enum TextEffect
    {
        Fill,
        Outline,
        Shadow,
        Glow,
        NumEffects
    };

    struct TextDeleter
    {
        void operator()( TTF_Text* ) const;
    };
    using TextPtr = std::unique_ptr<TTF_Text, TextDeleter>;

    struct SurfaceDeleter
    {
        void operator()( SDL_Surface* ) const;
    };
    using SurfacePtr = std::unique_ptr<SDL_Surface, SurfaceDeleter>;

    std::shared_ptr<const Font> m_Font;

    TextPtr m_Text[NumEffects];

    glm::ivec2 m_ShadowOffset { 0, 0 };
    int        m_GlowRadius = 0;

    // Set to true when the text content or properties have changed and the text needs to be re-rendered.
    mutable bool   m_IsDirty  = true;
    mutable size_t m_FontHash = 0;
    // Cached surface for the text. This is used to optimize rendering by avoiding redundant text rendering operations when the text content or properties haven't changed.
    mutable SurfacePtr m_CachedSurface;

    static void       setColor( const TextPtr& t, const Color& c );
    static Color      getColor( const TextPtr& t );
    static glm::ivec2 getSize( const TextPtr& t );
    static void       setWrapWidth( const TextPtr& t, int w );
};
}  // namespace graphics
}  // namespace sr

inline std::ostream& operator<<( std::ostream& os, const sr::graphics::Text& text )
{
    os << text.getText();
    return os;
}