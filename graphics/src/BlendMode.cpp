#include <Graphics/BlendMode.hpp>

using namespace sr::graphics;

const BlendMode BlendMode::Disable { false };
const BlendMode BlendMode::AlphaDiscard { true, 127 };
const BlendMode BlendMode::AlphaBlend { true, 0, BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha };
const BlendMode BlendMode::AdditiveBlend { true, 0, BlendFactor::One, BlendFactor::One };
const BlendMode BlendMode::SubtractiveBlend { true, 0, BlendFactor::One, BlendFactor::One, BlendOperation::Subtract };
const BlendMode BlendMode::MultiplicativeBlend { true, 0, BlendFactor::Zero, BlendFactor::SrcColor };
