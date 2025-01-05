#include "precomp.h"
#include "Player.h"
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
	score = 0;
	scoreMultiplier = 1.f;
}

void Score::resetMultiplier()
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
	currentWave = 1;
	currentState = InMenu;
	clusterNotification = new Audio::Sound("assets/audio/clusterNotification.wav", Audio::Sound::Type::Sound);
	clusterNotification->setVolume(0.5f);
}

void GameManager::clean() const
{
	delete score;
	delete clusterNotification;
	// Note: GameWorld takes care of the labels when it comes to cleaning
}

void GameManager::update()
{
	if (player->isDead())
	{
		// game over
		currentState = GameOver;
	}
}

void GameManager::reset() const
{
	score->reset();
	player->reset();

	// update ui
	updateLivesDisplay();
	score->updateScoreDisplay();
	score->updateMultiplierDisplay();

	clusterLabel->setText("");
}

void GameManager::updateLivesDisplay() const
{
	if (livesLabel)
	{
		livesLabel->setText(std::format("Lives: {}", player->getLives()));
	}
}

void GameManager::updateScoreMultiplierDisplay(float currentScoreMultiplier) const
{
	if (scoreMultiplierLabel)
	{
		scoreMultiplierLabel->setText(std::format("Multiplier: {:.1f}", currentScoreMultiplier));
	}
}

void GameManager::setWave(const int waveNumber)
{
	currentWave = waveNumber;
	updateWaveDisplay();
}

void GameManager::updateWaveDisplay() const
{
	if (waveLabel)
	{
		waveLabel->setText(std::format("Wave {}", currentWave));
	}
}

// cluster timer text
static const float TOTAL_TIME = 1.5f;
static const float RUNTIME = 0.3f;
static float timer = 0.0f;
static float runtimeTimer = 0.0f;
static bool showText = false;

void GameManager::updateClusterDisplay() const
{
	if (clusterLabel)
	{
		if (!clusterIncoming)
		{
			clusterLabel->setText("");
			return;
		}

		// make the notification blink
		timer += Time::deltaTime;
		runtimeTimer += Time::deltaTime;

		clusterLabel->setText(showText ? "Cluster Incoming!" : "");

		// reset
		if (timer >= RUNTIME)
		{
			clusterNotification->replay();
			showText = !showText;
			timer = 0.0f;
		}

		// stop after a set duration
		if (runtimeTimer >= TOTAL_TIME)
		{
			runtimeTimer = 0.0f;
			clusterIncoming = false;
		}
	}
}
