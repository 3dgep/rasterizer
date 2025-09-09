#include <Scores.hpp>

#include <fstream>
#include <iostream>

namespace
{
int P1Score { 1 };
int P2Score { 1 };
}  // namespace

void Scores::loadScores( const std::filesystem::path& scoreFile )
{
    std::ifstream scores { scoreFile };
    if ( !scores )
    {
        std::cerr << "Failed to open file for reading: " << scoreFile << std::endl;
        return;
    }

    scores >> P1Score;
    scores >> P2Score;

    if ( scores.fail() )
    {
        std::cerr << "Failed to read scores from file: " << scoreFile << std::endl;
    }
}

void Scores::saveScores( const std::filesystem::path& scoreFile )
{
    std::ofstream scores { scoreFile };
    if ( !scores )
    {
        std::cerr << "Failed to open file for writing: " << scoreFile << std::endl;
        return;
    }

    scores << P1Score << std::endl;
    scores << P2Score << std::endl;

    if ( scores.fail() )
    {
        std::cerr << "Failed to write scores to file: " << scoreFile << std::endl;
    }
}

int Scores::getP1Score()
{
    return P1Score;
}

int Scores::getP2Score()
{
    return P2Score;
}

int Scores::addP1Score()
{
    return ++P1Score;
}

int Scores::addP2Score()
{
    return ++P2Score;
}

void Scores::resetScores()
{
    P1Score = 0;
    P2Score = 0;
}
