#include "precomp.h"
#include "GameManager.h"

// -----------------------------------------------------------
// Score implementation
// -----------------------------------------------------------

Score::Score(Label* sL, Label* mL, const int initialScore, const float initialMultiplier)
	: score(initialScore), scoreMultiplier(initialMultiplier), scoreLabel(sL), multiplierLabel(mL)
{
	updateScoreDisplay();
	updateMultiplierDisplay();
}

void Score::reset()
{
	scoreMultiplier = 1.f;
}

void Score::addScore(const int v)
{
	score += static_cast<int>(static_cast<float>(v) * scoreMultiplier);
	updateScoreDisplay();
}

void Score::incrementMultiplier()
{
	scoreMultiplier += 0.2f;
	scoreMultiplier = std::min(scoreMultiplier, 5.0f);

	updateMultiplierDisplay();
}

void Score::updateScoreDisplay()
{
	if (scoreLabel)
	{
		scoreLabel->setText(std::format("Score: {}", score));
	}
}

void Score::updateMultiplierDisplay()
{
	if (multiplierLabel)
	{
		multiplierLabel->setText(std::format("Multiplier: {:.1f}", scoreMultiplier));
	}
}

// -----------------------------------------------------------
// Game Manager implementation
// -----------------------------------------------------------

GameManager& GameManager::instance()
{
	static GameManager s;
	return s;
}

void GameManager::instantiate()
{
	score = new Score{ scoreLabel, scoreMultiplierLabel };
}

void GameManager::clean()
{
	delete score;
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
