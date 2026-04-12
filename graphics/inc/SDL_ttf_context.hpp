#pragma once

struct TTF_TextEngine;

struct SDL_ttf_context
{
    static SDL_ttf_context& get();
    static TTF_TextEngine*  textEngine();

    // Singleton class. No copy or moves allowed.
    SDL_ttf_context( const SDL_ttf_context& )            = delete;
    SDL_ttf_context( SDL_ttf_context&& )                 = delete;
    SDL_ttf_context& operator=( const SDL_ttf_context& ) = delete;
    SDL_ttf_context& operator=( SDL_ttf_context&& )      = delete;

private:
    SDL_ttf_context();
    ~SDL_ttf_context();

    TTF_TextEngine* m_TextEngine = nullptr;
};
