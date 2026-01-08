#include <graphics/SamplerState.hpp>

using namespace sr::graphics;

const SamplerState SamplerState::WrapUnnormalized { AddressMode::Wrap, Color { 0xff000000 }, false };
const SamplerState SamplerState::WrapNormalized { AddressMode::Wrap, Color { 0xff000000 }, true };
const SamplerState SamplerState::MirrorUnnormalized { AddressMode::Mirror, Color { 0xff000000 }, false };
const SamplerState SamplerState::MirrorNormalized { AddressMode::Mirror, Color { 0xff000000 }, true };
const SamplerState SamplerState::ClampUnnormalized { AddressMode::Clamp, Color { 0xff000000 }, false };
const SamplerState SamplerState::ClampNormalized { AddressMode::Clamp, Color { 0xff000000 }, true };
const SamplerState SamplerState::BorderUnnormalized { AddressMode::Border, Color { 0xff000000 }, false };
const SamplerState SamplerState::BorderNormalized { AddressMode::Border, Color { 0xff000000 }, true };
