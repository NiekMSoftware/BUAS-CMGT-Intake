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
	void clean();

	// setting label
	void setScoreLabel(Label* label) { scoreLabel = label; }
	void setScoreMultiplierLabel(Label* label) { scoreMultiplierLabel = label; }
	void setLivesLabel(Label* label) { livesLabel = label; }

	void updateLivesDisplay(int currentLives) const;
	void updateScoreMultiplierDisplay(float currentScoreMultiplier) const;

	Score* score;

private:
	// Ui components
	Label* scoreLabel = nullptr;
	Label* scoreMultiplierLabel = nullptr;
	Label* livesLabel = nullptr;
};
