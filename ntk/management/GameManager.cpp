#include "precomp.h"
#include "GameManager.h"

#include <algorithm>

GameManager& GameManager::instance()
{
	static GameManager s;
	return s;
}

void GameManager::instantiate()
{
	score = 0;
	scoreMultiplier = 1.0f;
}

void GameManager::clean()
{
	scoreLabel = nullptr;
	delete scoreLabel;

	livesLabel = nullptr;
	delete livesLabel;

	scoreMultiplierLabel = nullptr;
	delete scoreMultiplierLabel;
}

void GameManager::addScore(const int v)
{
	score += static_cast<int>(static_cast<float>(v) * scoreMultiplier);

	// update label
	if (scoreLabel)
	{
		scoreLabel->setText(std::format("Score: {}", score));
	}
}

void GameManager::incrementMultiplier()
{
	scoreMultiplier += 0.2f;
	scoreMultiplier = std::min(scoreMultiplier, 5.0f);

	updateScoreMultiplierDisplay(scoreMultiplier);
}

void GameManager::resetScoreMultiplier()
{
	scoreMultiplier = 1.0f;
	updateScoreMultiplierDisplay(scoreMultiplier);
}

void GameManager::updateLivesDisplay(int currentLives) const
{
	if (livesLabel)
	{
		livesLabel->setText(std::format("Lives: {}", currentLives));
	}
}

void GameManager::updateScoreMultiplierDisplay(float currentScoreMultiplier) const
{
	if (scoreMultiplierLabel)
	{
		scoreMultiplierLabel->setText(std::format("Multiplier: {:.1f}", currentScoreMultiplier));
	}
}
