#pragma once

#include <memory>

struct TTF_TextEngine;

struct SDL_ttf_context
{
    static std::shared_ptr<SDL_ttf_context> get();

    TTF_TextEngine* textEngine = nullptr;

    SDL_ttf_context();
    ~SDL_ttf_context();
};
