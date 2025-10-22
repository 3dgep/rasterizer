#include <Character.hpp>

#include <iostream>

using namespace sr;

Character::Character( const Character& copy )
: anims { copy.anims }
{}

Character::Character( Character&& other ) noexcept
: anims { std::move( other.anims ) }
{}

Character& Character::operator=( const Character& copy )
{
    if ( this == &copy )
        return *this;

    anims       = copy.anims;
    currentAnim = nullptr;

    return *this;
}

Character& Character::operator=( Character&& other ) noexcept
{
    if ( this == &other )
        return *this;

    anims       = std::move( other.anims );
    currentAnim = nullptr;

    return *this;
}

void Character::addAnimation( std::string_view animName, SpriteAnimation anim )
{
    anims[std::string( animName )] = std::move( anim );
    currentAnim                    = nullptr;
}

void Character::setAnimation( std::string_view animName )
{
    const auto iter = anims.find( std::string( animName ) );

    if ( iter == anims.end() )
    {
        std::cerr << "Animation with name: " << animName << " not found in animations.";
    }
    else if ( currentAnim != &iter->second )
    {
        currentAnim = &iter->second;
        currentAnim->reset();
    }
}

const SpriteAnimation& Character::getAnimation( std::string_view animName ) const
{
    const auto iter = anims.find( std::string( animName ) );
    if ( iter != anims.end() )
        return iter->second;

    std::cerr << "Animation with name: " << animName << " not found in animations.";

    static SpriteAnimation emptyAnim;
    return emptyAnim;
}

void Character::update( float deltaTime )
{
    if ( currentAnim != nullptr )
    {
        currentAnim->update( deltaTime );
    }
}

void Character::draw( Rasterizer& rasterizer, const Transform2D& transform ) const
{
    if ( currentAnim != nullptr )
    {
        rasterizer.drawSprite( *currentAnim, transform );
    }
}
