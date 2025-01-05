#pragma once

struct Score
{
	Score(Label* sL, Label* mL, int initialScore = 0, float initialMultiplier = 1.0f);

	void resetMultiplier();
	void addScore(int v);
	void incrementMultiplier();

	int getCurrentScore() const { return score; }
	float getCurrentMultiplier() const { return scoreMultiplier; }

private:
	void updateScoreDisplay();
	void updateMultiplierDisplay();

	int score;
	float scoreMultiplier;

	Label* scoreLabel;
	Label* multiplierLabel;
};

class GameManager
{
public:
	static GameManager& instance();
	void instantiate();
	void clean() const;

	// setting label
	void setScoreLabel(Label* label) { scoreLabel = label; }
	void setScoreMultiplierLabel(Label* label) { scoreMultiplierLabel = label; }
	void setLivesLabel(Label* label) { livesLabel = label; }

	void setWaveLabel(Label* label) { waveLabel = label; }
	void setWave(int waveNumber);
	void setClusterLabel(Label* label) { clusterLabel = label; }

	void updateLivesDisplay(int currentLives) const;
	void updateScoreMultiplierDisplay(float currentScoreMultiplier) const;
	void updateWaveDisplay() const;
	void updateClusterDisplay() const;

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
};
