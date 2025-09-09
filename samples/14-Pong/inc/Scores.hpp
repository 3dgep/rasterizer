#pragma once

#include <filesystem>

namespace Scores
{
/// <summary>
/// Loads scores from the specified file path.
/// </summary>
/// <remarks>
/// The score file is a text file with only 2 lines:
/// <code>
/// <P1Score:int>
/// <P2Score:int>
/// EOF
/// </code>
/// </remarks>
/// <param name="scoreFile">The path to the file containing the scores to load.</param>
void loadScores( const std::filesystem::path& scoreFile );

/// <summary>
/// Saves scores to the specified file path.
/// </summary>
/// <param name="scoreFile">The path to the file where scores will be saved.</param>
void saveScores( const std::filesystem::path& scoreFile );

/// <summary>
/// Retrieves the score for player 1.
/// </summary>
/// <returns>The score of player 1 as an integer.</returns>
int getP1Score();

/// <summary>
/// Retrieves the score of player 2.
/// </summary>
/// <returns>The score of player 2 as an integer.</returns>
int getP2Score();

/// <summary>
/// Returns the current score of player 1 after adding a point.
/// </summary>
/// <returns>The updated score of player 1 as an integer.</returns>
int addP1Score();

/// <summary>
/// Returns the current score of player 2 after adding a point.
/// </summary>
/// <returns>The updated score of player 2 as an integer.</returns>
int addP2Score();

/// <summary>
/// Resets all scores to 0.
/// </summary>
void resetScores();
}