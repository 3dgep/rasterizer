#pragma once

#include <math/Rect.hpp>

#include <filesystem>
#include <vector>

// Parse the XML file and return a list of rectangles in the texture atlas.
std::vector<sr::math::RectI> parseRects( const std::filesystem::path& xmlFile );
