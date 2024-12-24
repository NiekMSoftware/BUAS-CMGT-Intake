#include "precomp.h"
#include "GameManager.h"

GameManager& GameManager::instance()
{
	static GameManager s;
	return s;
}

void GameManager::instantiate()
{
	score = 0;
}

void GameManager::clean()
{
	scoreLabel = nullptr;
	delete scoreLabel;

	livesLabel = nullptr;
	delete livesLabel;
}

void GameManager::addScore(const int v)
{
	score += v;

	// update label
	if (scoreLabel)
	{
		scoreLabel->setText(std::format("Score: {}", score));
	}
}

void GameManager::updateLivesDisplay(int currentLives)
{
	if (livesLabel)
	{
		livesLabel->setText(std::format("Lives: {}", currentLives));
	}
}
