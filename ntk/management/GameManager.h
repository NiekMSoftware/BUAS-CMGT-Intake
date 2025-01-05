#pragma once

class Player;

struct Score
{
	Score(Label* sL, Label* mL, int initialScore = 0, float initialMultiplier = 1.0f);

	void reset();
	void resetMultiplier();
	void addScore(int v);
	void incrementMultiplier();

	int getCurrentScore() const { return score; }
	float getCurrentMultiplier() const { return scoreMultiplier; }

	void updateScoreDisplay();
	void updateMultiplierDisplay();

private:
	int score;
	float scoreMultiplier;

	Label* scoreLabel;
	Label* multiplierLabel;
};

enum GameState
{
	InMenu,
	Playing,
	GameOver
};

class GameManager
{
public:
	static GameManager& instance();
	void instantiate();
	void clean() const;
	void update();
	void reset() const;

	// setting label
	void setScoreLabel(Label* label) { scoreLabel = label; }
	void setScoreMultiplierLabel(Label* label) { scoreMultiplierLabel = label; }
	void setLivesLabel(Label* label) { livesLabel = label; }

	void setWaveLabel(Label* label) { waveLabel = label; }
	void setWave(int waveNumber);
	void setClusterLabel(Label* label) { clusterLabel = label; }
	void setClusterIncoming(const bool value) const { clusterIncoming = value; }

	void updateLivesDisplay() const;
	void updateScoreMultiplierDisplay(float currentScoreMultiplier) const;
	void updateWaveDisplay() const;
	void updateClusterDisplay() const;

	void setPlayer(Player* p) { player = p; }
	Player* getPlayer() const { return player; }

	void setGameState(const GameState newState) { currentState = newState; }
	GameState getCurrentState() const { return currentState; }

	Score* score;

private:
	// Ui components
	Label* scoreLabel = nullptr;
	Label* scoreMultiplierLabel = nullptr;
	Label* livesLabel = nullptr;

	int currentWave = 1;
	Label* waveLabel = nullptr;

	Label* clusterLabel = nullptr;
	mutable float blinkTimer = 0.0f;

	Player* player = nullptr;
	GameState currentState = InMenu;

	mutable bool clusterIncoming = false;
	Audio::Sound* clusterNotification = nullptr;
};
