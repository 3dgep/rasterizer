#pragma once

#include "Color.hpp"
#include "Font.hpp"

struct TTF_Text;
struct TTF_TextEngine;

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

    Text()              = default;
    explicit Text( Font font, std::string_view text = {} );
    Text( const Text& ) = delete;
    Text( Text&& ) noexcept;
    ~Text();

    Text& operator=( const Text& ) = delete;
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
    Text& setString( std::string_view string );

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
    Color getColor() const;

    /// <summary>
    /// Sets the color of the text object.
    /// </summary>
    /// <param name="color">The color to apply to the text.</param>
    /// <returns>A reference to the modified Text object.</returns>
    Text& setColor( const Color& color );

    /// <summary>
    /// Set the direction to be used for text shaping a text object.
    /// </summary>
    /// <param name="direction">The direction for the text to flow.</param>
    /// <returns>A reference to the modified text object.</returns>
    Text& setDirection( Direction direction );

    /// <summary>
    /// Get the direction to be used for text shaping a text object.
    /// </summary>
    /// <returns>The direction the text will flow when rendering.</returns>
    Direction getDirection() const;

    /// <summary>
    /// Get the font used by the text object.
    /// </summary>
    /// <returns>The font used by the text object.</returns>
    const Font& getFont() const;

    /// <summary>
    /// Sets the font used by the text object.
    /// </summary>
    /// <param name="font">A reference to the Font object to be used for rendering the text.</param>
    /// <returns>A reference to the modified Text object.</returns>
    Text& setFont( const Font& font );

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
    glm::ivec2 getSize() const;

    int getWidth() const;

    int getHeight() const;

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

    // For internal use.
    TTF_Text* getTTF_Text() const
    {
        return m_Text;
    }

    // Get a pointer to the text engine used for rendering text to a surface.
    static TTF_TextEngine* TextEngine();

private:
    Font      m_Font;
    TTF_Text* m_Text = nullptr;
};
}  // namespace graphics
}  // namespace sr