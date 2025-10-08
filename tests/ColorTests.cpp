#include <graphics/Color.hpp>
#include <gtest/gtest.h>

using namespace sr::graphics;

// Test #rgb format (3-digit hex)
TEST(ColorFromHTMLTest, ShortHexFormat)
{
    // #rgb format expands each digit: #rgb -> #rrggbb
    Color color = Color::fromHTML("#f00");
    EXPECT_EQ(color.r, 255);  // f -> ff
    EXPECT_EQ(color.g, 0);
    EXPECT_EQ(color.b, 0);
    EXPECT_EQ(color.a, 255);

    color = Color::fromHTML("#0f0");
    EXPECT_EQ(color.r, 0);
    EXPECT_EQ(color.g, 255);  // f -> ff
    EXPECT_EQ(color.b, 0);
    EXPECT_EQ(color.a, 255);

    color = Color::fromHTML("#00f");
    EXPECT_EQ(color.r, 0);
    EXPECT_EQ(color.g, 0);
    EXPECT_EQ(color.b, 255);  // f -> ff
    EXPECT_EQ(color.a, 255);

    color = Color::fromHTML("#abc");
    EXPECT_EQ(color.r, 170);  // a -> aa
    EXPECT_EQ(color.g, 187);  // b -> bb
    EXPECT_EQ(color.b, 204);  // c -> cc
    EXPECT_EQ(color.a, 255);
}

// Test #rrggbb format (6-digit hex)
TEST(ColorFromHTMLTest, LongHexFormat)
{
    Color color = Color::fromHTML("#ff0000");
    EXPECT_EQ(color.r, 255);
    EXPECT_EQ(color.g, 0);
    EXPECT_EQ(color.b, 0);
    EXPECT_EQ(color.a, 255);

    color = Color::fromHTML("#00ff00");
    EXPECT_EQ(color.r, 0);
    EXPECT_EQ(color.g, 255);
    EXPECT_EQ(color.b, 0);
    EXPECT_EQ(color.a, 255);

    color = Color::fromHTML("#0000ff");
    EXPECT_EQ(color.r, 0);
    EXPECT_EQ(color.g, 0);
    EXPECT_EQ(color.b, 255);
    EXPECT_EQ(color.a, 255);

    color = Color::fromHTML("#abcdef");
    EXPECT_EQ(color.r, 171);
    EXPECT_EQ(color.g, 205);
    EXPECT_EQ(color.b, 239);
    EXPECT_EQ(color.a, 255);
}

// Test #rrggbbaa format (8-digit hex with alpha)
TEST(ColorFromHTMLTest, LongHexFormatWithAlpha)
{
    Color color = Color::fromHTML("#ff000080");
    EXPECT_EQ(color.r, 255);
    EXPECT_EQ(color.g, 0);
    EXPECT_EQ(color.b, 0);
    EXPECT_EQ(color.a, 128);

    color = Color::fromHTML("#00ff00ff");
    EXPECT_EQ(color.r, 0);
    EXPECT_EQ(color.g, 255);
    EXPECT_EQ(color.b, 0);
    EXPECT_EQ(color.a, 255);

    color = Color::fromHTML("#0000ff00");
    EXPECT_EQ(color.r, 0);
    EXPECT_EQ(color.g, 0);
    EXPECT_EQ(color.b, 255);
    EXPECT_EQ(color.a, 0);

    color = Color::fromHTML("#abcdef12");
    EXPECT_EQ(color.r, 171);
    EXPECT_EQ(color.g, 205);
    EXPECT_EQ(color.b, 239);
    EXPECT_EQ(color.a, 18);
}

// Test rgb(r, g, b) format
TEST(ColorFromHTMLTest, RGBFormat)
{
    Color color = Color::fromHTML("rgb(255, 0, 0)");
    EXPECT_EQ(color.r, 255);
    EXPECT_EQ(color.g, 0);
    EXPECT_EQ(color.b, 0);
    EXPECT_EQ(color.a, 255);

    color = Color::fromHTML("rgb(0, 255, 0)");
    EXPECT_EQ(color.r, 0);
    EXPECT_EQ(color.g, 255);
    EXPECT_EQ(color.b, 0);
    EXPECT_EQ(color.a, 255);

    color = Color::fromHTML("rgb(0, 0, 255)");
    EXPECT_EQ(color.r, 0);
    EXPECT_EQ(color.g, 0);
    EXPECT_EQ(color.b, 255);
    EXPECT_EQ(color.a, 255);

    // Test with extra whitespace
    color = Color::fromHTML("rgb( 128 , 64 , 192 )");
    EXPECT_EQ(color.r, 128);
    EXPECT_EQ(color.g, 64);
    EXPECT_EQ(color.b, 192);
    EXPECT_EQ(color.a, 255);

    // Test case insensitivity
    color = Color::fromHTML("RGB(100, 150, 200)");
    EXPECT_EQ(color.r, 100);
    EXPECT_EQ(color.g, 150);
    EXPECT_EQ(color.b, 200);
    EXPECT_EQ(color.a, 255);
}

// Test rgba(r, g, b, a) format
TEST(ColorFromHTMLTest, RGBAFormat)
{
    Color color = Color::fromHTML("rgba(255, 0, 0, 0.5)");
    EXPECT_EQ(color.r, 255);
    EXPECT_EQ(color.g, 0);
    EXPECT_EQ(color.b, 0);
    EXPECT_EQ(color.a, 127);  // 0.5 * 255 = 127.5, rounds to 127

    color = Color::fromHTML("rgba(0, 255, 0, 1.0)");
    EXPECT_EQ(color.r, 0);
    EXPECT_EQ(color.g, 255);
    EXPECT_EQ(color.b, 0);
    EXPECT_EQ(color.a, 255);

    color = Color::fromHTML("rgba(0, 0, 255, 0)");
    EXPECT_EQ(color.r, 0);
    EXPECT_EQ(color.g, 0);
    EXPECT_EQ(color.b, 255);
    EXPECT_EQ(color.a, 0);

    // Test with extra whitespace
    color = Color::fromHTML("rgba( 128 , 64 , 192 , 0.25 )");
    EXPECT_EQ(color.r, 128);
    EXPECT_EQ(color.g, 64);
    EXPECT_EQ(color.b, 192);
    EXPECT_EQ(color.a, 63);  // 0.25 * 255 = 63.75, rounds to 63

    // Test case insensitivity
    color = Color::fromHTML("RGBA(100, 150, 200, 0.8)");
    EXPECT_EQ(color.r, 100);
    EXPECT_EQ(color.g, 150);
    EXPECT_EQ(color.b, 200);
    EXPECT_EQ(color.a, 204);  // 0.8 * 255 = 204
}

// Test hsl(h, s, l) format
TEST(ColorFromHTMLTest, HSLFormat)
{
    // Red: hsl(0, 100%, 50%)
    Color color = Color::fromHTML("hsl(0, 100%, 50%)");
    EXPECT_EQ(color.r, 255);
    EXPECT_EQ(color.g, 0);
    EXPECT_EQ(color.b, 0);
    EXPECT_EQ(color.a, 255);

    // Green: hsl(120, 100%, 50%)
    color = Color::fromHTML("hsl(120, 100%, 50%)");
    EXPECT_EQ(color.r, 0);
    EXPECT_EQ(color.g, 255);
    EXPECT_EQ(color.b, 0);
    EXPECT_EQ(color.a, 255);

    // Blue: hsl(240, 100%, 50%)
    color = Color::fromHTML("hsl(240, 100%, 50%)");
    EXPECT_EQ(color.r, 0);
    EXPECT_EQ(color.g, 0);
    EXPECT_EQ(color.b, 255);
    EXPECT_EQ(color.a, 255);

    // White: hsl(0, 0%, 100%)
    color = Color::fromHTML("hsl(0, 0%, 100%)");
    EXPECT_EQ(color.r, 255);
    EXPECT_EQ(color.g, 255);
    EXPECT_EQ(color.b, 255);
    EXPECT_EQ(color.a, 255);

    // Black: hsl(0, 0%, 0%)
    color = Color::fromHTML("hsl(0, 0%, 0%)");
    EXPECT_EQ(color.r, 0);
    EXPECT_EQ(color.g, 0);
    EXPECT_EQ(color.b, 0);
    EXPECT_EQ(color.a, 255);

    // Test with whitespace and case insensitivity
    color = Color::fromHTML("HSL( 60 , 100% , 50% )");
    EXPECT_EQ(color.r, 255);
    EXPECT_EQ(color.g, 255);
    EXPECT_EQ(color.b, 0);
    EXPECT_EQ(color.a, 255);
}

// Test named colors
TEST(ColorFromHTMLTest, NamedColors)
{
    // Test basic colors
    Color color = Color::fromHTML("red");
    EXPECT_EQ(color, Color::Red);

    color = Color::fromHTML("green");
    EXPECT_EQ(color, Color::Green);

    color = Color::fromHTML("blue");
    EXPECT_EQ(color, Color::Blue);

    color = Color::fromHTML("white");
    EXPECT_EQ(color, Color::White);

    color = Color::fromHTML("black");
    EXPECT_EQ(color, Color::Black);

    // Test case insensitivity
    color = Color::fromHTML("RED");
    EXPECT_EQ(color, Color::Red);

    color = Color::fromHTML("GrEeN");
    EXPECT_EQ(color, Color::Green);

    color = Color::fromHTML("BlUe");
    EXPECT_EQ(color, Color::Blue);

    // Test more complex color names
    color = Color::fromHTML("cornflowerblue");
    EXPECT_EQ(color, Color::CornflowerBlue);

    color = Color::fromHTML("darkgoldenrod");
    EXPECT_EQ(color, Color::DarkGoldenRod);

    color = Color::fromHTML("lightsteelblue");
    EXPECT_EQ(color, Color::LightSteelBlue);
}

// Test whitespace handling
TEST(ColorFromHTMLTest, WhitespaceHandling)
{
    Color color = Color::fromHTML("  #ff0000  ");
    EXPECT_EQ(color.r, 255);
    EXPECT_EQ(color.g, 0);
    EXPECT_EQ(color.b, 0);

    color = Color::fromHTML("\t\nred\r\n");
    EXPECT_EQ(color, Color::Red);
}

// Test invalid inputs (should return black)
TEST(ColorFromHTMLTest, InvalidInputs)
{
    Color color = Color::fromHTML("invalid");
    EXPECT_EQ(color, Color::Black);

    color = Color::fromHTML("#xyz");
    EXPECT_EQ(color, Color::Black);

    color = Color::fromHTML("rgb(300, 400, 500)");  // Values clamped, should still parse
    EXPECT_EQ(color.r, 255);
    EXPECT_EQ(color.g, 255);
    EXPECT_EQ(color.b, 255);
    EXPECT_EQ(color.a, 255);

    color = Color::fromHTML("");
    EXPECT_EQ(color, Color::Black);
}
