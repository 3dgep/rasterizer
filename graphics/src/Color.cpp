#include <graphics/Color.hpp>

#include <algorithm>
#include <cctype>
#include <regex>
#include <string>
#include <unordered_map>

using namespace sr::graphics;

const Color Color::AliceBlue { 0xfffff8f0 };
const Color Color::AntiqueWhite { 0xffd7ebfa };
const Color Color::Aqua { 0xffffff00 };
const Color Color::AquaMarine { 0xffd4ff7f };
const Color Color::Azure { 0xfffffff0 };
const Color Color::Beige { 0xffdcf5f5 };
const Color Color::Bisque { 0xffc4e4ff };
const Color Color::Black { 0xff000000 };
const Color Color::BlanchedAlmond { 0xffcdebff };
const Color Color::Blue { 0xffff0000 };
const Color Color::BlueViolet { 0xffe22b8a };
const Color Color::Brown { 0xff2a2aa5 };
const Color Color::BurlyWood { 0xff87b8de };
const Color Color::CadetBlue { 0xffa09e5f };
const Color Color::Chartreuse { 0xff00ff7f };
const Color Color::Chocolate { 0xff1e69d2 };
const Color Color::Coral { 0xff507fff };
const Color Color::CornflowerBlue { 0xffed9564 };
const Color Color::Cornsilk { 0xffdcf8ff };
const Color Color::Crimson { 0xff3c14dc };
const Color Color::Cyan { 0xffffff00 };
const Color Color::DarkBlue { 0xff8b0000 };
const Color Color::DarkCyan { 0xff8b8b00 };
const Color Color::DarkGoldenRod { 0xff0b86b8 };
const Color Color::DarkGray { 0xffa9a9a9 };
const Color Color::DarkGrey { 0xffa9a9a9 };
const Color Color::DarkGreen { 0xff006400 };
const Color Color::DarkKhaki { 0xff6bb7bd };
const Color Color::DarkMagenta { 0xff8b008b };
const Color Color::DarkOliveGreen { 0xff2f6b55 };
const Color Color::DarkOrange { 0xff008cff };
const Color Color::DarkOrchid { 0xffcc3299 };
const Color Color::DarkRed { 0xff00008b };
const Color Color::DarkSalmon { 0xff7a96e9 };
const Color Color::DarkSeaGreen { 0xff8bbc8f };
const Color Color::DarkSlateBlue { 0xff8b3d48 };
const Color Color::DarkSlateGray { 0xff4f4f2f };
const Color Color::DarkSlateGrey { 0xff4f4f2f };
const Color Color::DarkTurquoise { 0xffd1ce00 };
const Color Color::DarkViolet { 0xffd30094 };
const Color Color::DeepPink { 0xff9314ff };
const Color Color::DeepSkyBlue { 0xffffbf00 };
const Color Color::DimGray { 0xff696969 };
const Color Color::DimGrey { 0xff696969 };
const Color Color::DodgerBlue { 0xffff901e };
const Color Color::FireBrick { 0xff2222b2 };
const Color Color::FloralWhite { 0xfff0faff };
const Color Color::ForestGreen { 0xff228b22 };
const Color Color::Fuchsia { 0xffff00ff };
const Color Color::Gainsboro { 0xffdcdcdc };
const Color Color::GhostWhite { 0xfffff8f8 };
const Color Color::Gold { 0xff00d7ff };
const Color Color::GoldenRod { 0xff20a5da };
const Color Color::Gray { 0xff808080 };
const Color Color::Grey { 0xff808080 };
const Color Color::Green { 0xff008000 };
const Color Color::GreenYellow { 0xff2fffad };
const Color Color::HoneyDew { 0xfff0fff0 };
const Color Color::HotPink { 0xffb469ff };
const Color Color::IndianRed { 0xff5c5ccd };
const Color Color::Indigo { 0xff82004b };
const Color Color::Ivory { 0xfff0ffff };
const Color Color::Khaki { 0xff8ce6f0 };
const Color Color::Lavender { 0xfffae6e6 };
const Color Color::LavenderBlush { 0xfff5f0ff };
const Color Color::LawnGreen { 0xff00fc7c };
const Color Color::LemonChiffon { 0xffcdfaff };
const Color Color::LightBlue { 0xffe6d8ad };
const Color Color::LightCoral { 0xff8080f0 };
const Color Color::LightCyan { 0xffffffe0 };
const Color Color::LightGoldenRodYellow { 0xffd2fafa };
const Color Color::LightGray { 0xffd3d3d3 };
const Color Color::LightGrey { 0xffd3d3d3 };
const Color Color::LightGreen { 0xff90ee90 };
const Color Color::LightPink { 0xffc1b6ff };
const Color Color::LightSalmon { 0xff7aa0ff };
const Color Color::LightSeaGreen { 0xffaab220 };
const Color Color::LightSkyBlue { 0xffface87 };
const Color Color::LightSlateGray { 0xff998877 };
const Color Color::LightSlateGrey { 0xff998877 };
const Color Color::LightSteelBlue { 0xffdec4b0 };
const Color Color::LightYellow { 0xffe0ffff };
const Color Color::Lime { 0xff00ff00 };
const Color Color::LimeGreen { 0xff32cd32 };
const Color Color::Linen { 0xffe6f0fa };
const Color Color::Magenta { 0xffff00ff };
const Color Color::Maroon { 0xff000080 };
const Color Color::MediumAquaMarine { 0xffaacd66 };
const Color Color::MediumBlue { 0xffcd0000 };
const Color Color::MediumOrchid { 0xffd355ba };
const Color Color::MediumPurple { 0xffdb7093 };
const Color Color::MediumSeaGreen { 0xff71b33c };
const Color Color::MediumSlateBlue { 0xffee687b };
const Color Color::MediumSpringGreen { 0xff9afa00 };
const Color Color::MediumTurquoise { 0xffccd148 };
const Color Color::MediumVioletRed { 0xff8515c7 };
const Color Color::MidnightBlue { 0xff701919 };
const Color Color::MintCream { 0xfffafff5 };
const Color Color::MistyRose { 0xffe1e4ff };
const Color Color::Moccasin { 0xffb5e4ff };
const Color Color::NavajoWhite { 0xffaddeff };
const Color Color::Navy { 0xff800000 };
const Color Color::OldLace { 0xffe6f5fd };
const Color Color::Olive { 0xff008080 };
const Color Color::OliveDrab { 0xff238e6b };
const Color Color::Orange { 0xff00a5ff };
const Color Color::OrangeRed { 0xff0045ff };
const Color Color::Orchid { 0xffd670da };
const Color Color::PaleGoldenRod { 0xffaae8ee };
const Color Color::PaleGreen { 0xff98fb98 };
const Color Color::PaleTurquoise { 0xffeeeeaf };
const Color Color::PaleVioletRed { 0xff9370db };
const Color Color::PapayaWhip { 0xffd5efff };
const Color Color::PeachPuff { 0xffb9daff };
const Color Color::Peru { 0xff3f85cd };
const Color Color::Pink { 0xffcbc0ff };
const Color Color::Plum { 0xffdda0dd };
const Color Color::PowderBlue { 0xffe6e0b0 };
const Color Color::Purple { 0xff800080 };
const Color Color::RebeccaPurple { 0xff993366 };
const Color Color::Red { 0xff0000ff };
const Color Color::RosyBrown { 0xff8f8fbc };
const Color Color::RoyalBlue { 0xffe16941 };
const Color Color::SaddleBrown { 0xff13458b };
const Color Color::Salmon { 0xff7280fa };
const Color Color::SandyBrown { 0xff60a4f4 };
const Color Color::SeaGreen { 0xff578b2e };
const Color Color::Seashell { 0xffeef5ff };
const Color Color::Sienna { 0xff2d52a0 };
const Color Color::Silver { 0xffc0c0c0 };
const Color Color::SkyBlue { 0xffebce87 };
const Color Color::SlateBlue { 0xffcd5a6a };
const Color Color::SlateGray { 0xff908070 };
const Color Color::SlateGrey { 0xff908070 };
const Color Color::Snow { 0xfffafaff };
const Color Color::SpringGreen { 0xff7fff00 };
const Color Color::SteelBlue { 0xffb48246 };
const Color Color::Tan { 0xff8cb4d2 };
const Color Color::Teal { 0xff808000 };
const Color Color::Thistle { 0xffd8bfd8 };
const Color Color::Tomato { 0xff4763ff };
const Color Color::Turquoise { 0xffd0e040 };
const Color Color::Violet { 0xffee82ee };
const Color Color::Wheat { 0xffb3def5 };
const Color Color::White { 0xffffffff };
const Color Color::WhiteSmoke { 0xfff5f5f5 };
const Color Color::Yellow { 0xff00ffff };
const Color Color::YellowGreen { 0xff32cd9a };

    // Static map for color name lookups (initialized once)
static const std::unordered_map<std::string, Color> colorMap = {
    { "aliceblue", Color::AliceBlue },
    { "antiquewhite", Color::AntiqueWhite },
    { "aqua", Color::Aqua },
    { "aquamarine", Color::AquaMarine },
    { "azure", Color::Azure },
    { "beige", Color::Beige },
    { "bisque", Color::Bisque },
    { "black", Color::Black },
    { "blanchedalmond", Color::BlanchedAlmond },
    { "blue", Color::Blue },
    { "blueviolet", Color::BlueViolet },
    { "brown", Color::Brown },
    { "burlywood", Color::BurlyWood },
    { "cadetblue", Color::CadetBlue },
    { "chartreuse", Color::Chartreuse },
    { "chocolate", Color::Chocolate },
    { "coral", Color::Coral },
    { "cornflowerblue", Color::CornflowerBlue },
    { "cornsilk", Color::Cornsilk },
    { "crimson", Color::Crimson },
    { "cyan", Color::Cyan },
    { "darkblue", Color::DarkBlue },
    { "darkcyan", Color::DarkCyan },
    { "darkgoldenrod", Color::DarkGoldenRod },
    { "darkgray", Color::DarkGray },
    { "darkgrey", Color::DarkGrey },
    { "darkgreen", Color::DarkGreen },
    { "darkkhaki", Color::DarkKhaki },
    { "darkmagenta", Color::DarkMagenta },
    { "darkolivegreen", Color::DarkOliveGreen },
    { "darkorange", Color::DarkOrange },
    { "darkorchid", Color::DarkOrchid },
    { "darkred", Color::DarkRed },
    { "darksalmon", Color::DarkSalmon },
    { "darkseagreen", Color::DarkSeaGreen },
    { "darkslateblue", Color::DarkSlateBlue },
    { "darkslategray", Color::DarkSlateGray },
    { "darkslategrey", Color::DarkSlateGrey },
    { "darkturquoise", Color::DarkTurquoise },
    { "darkviolet", Color::DarkViolet },
    { "deeppink", Color::DeepPink },
    { "deepskyblue", Color::DeepSkyBlue },
    { "dimgray", Color::DimGray },
    { "dimgrey", Color::DimGrey },
    { "dodgerblue", Color::DodgerBlue },
    { "firebrick", Color::FireBrick },
    { "floralwhite", Color::FloralWhite },
    { "forestgreen", Color::ForestGreen },
    { "fuchsia", Color::Fuchsia },
    { "gainsboro", Color::Gainsboro },
    { "ghostwhite", Color::GhostWhite },
    { "gold", Color::Gold },
    { "goldenrod", Color::GoldenRod },
    { "gray", Color::Gray },
    { "grey", Color::Grey },
    { "green", Color::Green },
    { "greenyellow", Color::GreenYellow },
    { "honeydew", Color::HoneyDew },
    { "hotpink", Color::HotPink },
    { "indianred", Color::IndianRed },
    { "indigo", Color::Indigo },
    { "ivory", Color::Ivory },
    { "khaki", Color::Khaki },
    { "lavender", Color::Lavender },
    { "lavenderblush", Color::LavenderBlush },
    { "lawngreen", Color::LawnGreen },
    { "lemonchiffon", Color::LemonChiffon },
    { "lightblue", Color::LightBlue },
    { "lightcoral", Color::LightCoral },
    { "lightcyan", Color::LightCyan },
    { "lightgoldenrodyellow", Color::LightGoldenRodYellow },
    { "lightgray", Color::LightGray },
    { "lightgrey", Color::LightGrey },
    { "lightgreen", Color::LightGreen },
    { "lightpink", Color::LightPink },
    { "lightsalmon", Color::LightSalmon },
    { "lightseagreen", Color::LightSeaGreen },
    { "lightskyblue", Color::LightSkyBlue },
    { "lightslategray", Color::LightSlateGray },
    { "lightslategrey", Color::LightSlateGrey },
    { "lightsteelblue", Color::LightSteelBlue },
    { "lightyellow", Color::LightYellow },
    { "lime", Color::Lime },
    { "limegreen", Color::LimeGreen },
    { "linen", Color::Linen },
    { "magenta", Color::Magenta },
    { "maroon", Color::Maroon },
    { "mediumaquamarine", Color::MediumAquaMarine },
    { "mediumblue", Color::MediumBlue },
    { "mediumorchid", Color::MediumOrchid },
    { "mediumpurple", Color::MediumPurple },
    { "mediumseagreen", Color::MediumSeaGreen },
    { "mediumslateblue", Color::MediumSlateBlue },
    { "mediumspringgreen", Color::MediumSpringGreen },
    { "mediumturquoise", Color::MediumTurquoise },
    { "mediumvioletred", Color::MediumVioletRed },
    { "midnightblue", Color::MidnightBlue },
    { "mintcream", Color::MintCream },
    { "mistyrose", Color::MistyRose },
    { "moccasin", Color::Moccasin },
    { "navajowhite", Color::NavajoWhite },
    { "navy", Color::Navy },
    { "oldlace", Color::OldLace },
    { "olive", Color::Olive },
    { "olivedrab", Color::OliveDrab },
    { "orange", Color::Orange },
    { "orangered", Color::OrangeRed },
    { "orchid", Color::Orchid },
    { "palegoldenrod", Color::PaleGoldenRod },
    { "palegreen", Color::PaleGreen },
    { "paleturquoise", Color::PaleTurquoise },
    { "palevioletred", Color::PaleVioletRed },
    { "papayawhip", Color::PapayaWhip },
    { "peachpuff", Color::PeachPuff },
    { "peru", Color::Peru },
    { "pink", Color::Pink },
    { "plum", Color::Plum },
    { "powderblue", Color::PowderBlue },
    { "purple", Color::Purple },
    { "rebeccapurple", Color::RebeccaPurple },
    { "red", Color::Red },
    { "rosybrown", Color::RosyBrown },
    { "royalblue", Color::RoyalBlue },
    { "saddlebrown", Color::SaddleBrown },
    { "salmon", Color::Salmon },
    { "sandybrown", Color::SandyBrown },
    { "seagreen", Color::SeaGreen },
    { "seashell", Color::Seashell },
    { "sienna", Color::Sienna },
    { "silver", Color::Silver },
    { "skyblue", Color::SkyBlue },
    { "slateblue", Color::SlateBlue },
    { "slategray", Color::SlateGray },
    { "slategrey", Color::SlateGrey },
    { "snow", Color::Snow },
    { "springgreen", Color::SpringGreen },
    { "steelblue", Color::SteelBlue },
    { "tan", Color::Tan },
    { "teal", Color::Teal },
    { "thistle", Color::Thistle },
    { "tomato", Color::Tomato },
    { "turquoise", Color::Turquoise },
    { "violet", Color::Violet },
    { "wheat", Color::Wheat },
    { "white", Color::White },
    { "whitesmoke", Color::WhiteSmoke },
    { "yellow", Color::Yellow },
    { "yellowgreen", Color::YellowGreen }
};

Color Color::fromHTML( std::string_view html )
{
    // Convert string_view to string for regex processing
    std::string htmlStr( html );

    // Remove leading/trailing whitespace
    htmlStr.erase( 0, htmlStr.find_first_not_of( " \t\n\r\f\v" ) );
    htmlStr.erase( htmlStr.find_last_not_of( " \t\n\r\f\v" ) + 1 );

    // Regex patterns for hex colors
    // Matches: #RGB, #RRGGBB, #RRGGBBAA
    static const std::regex hexPattern( R"(^#([0-9A-Fa-f]{3}|[0-9A-Fa-f]{6}|[0-9A-Fa-f]{8})$)" );
    
    // Regex patterns for rgb/rgba
    // Matches: rgb(r, g, b) or rgba(r, g, b, a)
    static const std::regex rgbPattern( R"(^rgba?\(\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)\s*(?:,\s*([\d.]+)\s*)?\)$)", std::regex::icase );
    
    // Regex pattern for hsl/hsla
    // Matches: hsl(h, s%, l%) or hsla(h, s%, l%, a)
    static const std::regex hslPattern( R"(^hsla?\(\s*([\d.]+)\s*,\s*([\d.]+)%\s*,\s*([\d.]+)%\s*(?:,\s*([\d.]+)\s*)?\)$)", std::regex::icase );
    
    std::smatch match;
    
    // Try hex pattern first
    if ( std::regex_match( htmlStr, match, hexPattern ) )
    {
        std::string hexStr = match[1].str();
        
        if ( hexStr.length() == 3 )
        {
            // #RGB -> #RRGGBB
            const uint8_t r = static_cast<uint8_t>( std::stoi( std::string( 2, hexStr[0] ), nullptr, 16 ) );
            const uint8_t g = static_cast<uint8_t>( std::stoi( std::string( 2, hexStr[1] ), nullptr, 16 ) );
            const uint8_t b = static_cast<uint8_t>( std::stoi( std::string( 2, hexStr[2] ), nullptr, 16 ) );
            return { r, g, b, 255 };
        }
        if ( hexStr.length() == 6 )
        {
            // #RRGGBB
            const uint8_t r = static_cast<uint8_t>( std::stoi( hexStr.substr( 0, 2 ), nullptr, 16 ) );
            const uint8_t g = static_cast<uint8_t>( std::stoi( hexStr.substr( 2, 2 ), nullptr, 16 ) );
            const uint8_t b = static_cast<uint8_t>( std::stoi( hexStr.substr( 4, 2 ), nullptr, 16 ) );
            return { r, g, b, 255 };
        }
        if ( hexStr.length() == 8 )
        {
            // #RRGGBBAA
            const uint8_t r = static_cast<uint8_t>( std::stoi( hexStr.substr( 0, 2 ), nullptr, 16 ) );
            const uint8_t g = static_cast<uint8_t>( std::stoi( hexStr.substr( 2, 2 ), nullptr, 16 ) );
            const uint8_t b = static_cast<uint8_t>( std::stoi( hexStr.substr( 4, 2 ), nullptr, 16 ) );
            const uint8_t a = static_cast<uint8_t>( std::stoi( hexStr.substr( 6, 2 ), nullptr, 16 ) );
            return { r, g, b, a };
        }
    }

    // Try rgb/rgba pattern
    if ( std::regex_match( htmlStr, match, rgbPattern ) )
    {
        const int r = std::stoi( match[1].str() );
        const int g = std::stoi( match[2].str() );
        const int b = std::stoi( match[3].str() );
        
        // Clamp values to 0-255
        const uint8_t red   = static_cast<uint8_t>( std::clamp( r, 0, 255 ) );
        const uint8_t green = static_cast<uint8_t>( std::clamp( g, 0, 255 ) );
        const uint8_t blue  = static_cast<uint8_t>( std::clamp( b, 0, 255 ) );
        
        // Handle optional alpha
        uint8_t alpha = 255;
        if ( match[4].matched )
        {
            const float a = std::stof( match[4].str() );
            alpha = static_cast<uint8_t>( std::clamp( a * 255.0f, 0.0f, 255.0f ) );
        }
        
        return { red, green, blue, alpha };
    }

    // Try hsl/hsla pattern
    if ( std::regex_match( htmlStr, match, hslPattern ) )
    {
        const float h = std::stof( match[1].str() );
        const float s = std::stof( match[2].str() ) / 100.0f; // Convert percentage to 0-1
        const float l = std::stof( match[3].str() ) / 100.0f; // Convert percentage to 0-1
        
        // Clamp values
        const float hue        = std::fmod( h, 360.0f );
        const float saturation = std::clamp( s, 0.0f, 1.0f );
        const float lightness  = std::clamp( l, 0.0f, 1.0f );
        
        // Convert HSL to HSV
        // Reference: https://en.wikipedia.org/wiki/HSL_and_HSV#HSL_to_HSV
        const float value = lightness + saturation * std::min( lightness, 1.0f - lightness );
        const float saturationHSV = value == 0.0f ? 0.0f : 2.0f * ( 1.0f - lightness / value );
        
        // Use the existing fromHSV function
        Color color = fromHSV( hue, saturationHSV, value );
        
        // Handle optional alpha
        if ( match[4].matched )
        {
            const float a = std::stof( match[4].str() );
            color = color.withAlpha( a );
        }
        
        return color;
    }

    // Handle named colors (case-insensitive)
    std::string lower;
    lower.reserve( htmlStr.length() );
    for ( char c : htmlStr )
        lower += static_cast<char>( std::tolower( static_cast<unsigned char>( c ) ) );

    // Lookup in the color map
    auto it = colorMap.find( lower );
    if ( it != colorMap.end() )
        return it->second;

    // Unknown color, return black
    return Color::Black;
}