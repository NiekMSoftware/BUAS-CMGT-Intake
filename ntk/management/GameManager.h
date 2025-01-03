#pragma once

class GameManager
{
public:
	static GameManager& instance();
	void instantiate();
	void clean();

	// setting labels
	void setScoreLabel(Label* label) { scoreLabel = label; }
	void setScoreMultiplierLabel(Label* label) { scoreMultiplierLabel = label; }
	void setLivesLabel(Label* label) { livesLabel = label; }

	// scoring
	void addScore(int v);
	int getScore() const { return score; }
	float getScoreMultiplier() const { return scoreMultiplier; }
	void incrementMultiplier();
	void resetScoreMultiplier();

	void updateLivesDisplay(int currentLives) const;
	void updateScoreMultiplierDisplay(float currentScoreMultiplier) const;

private:
	int score = 0;
	float scoreMultiplier = 0.0f;

	// Ui components
	Label* scoreLabel = nullptr;
	Label* scoreMultiplierLabel = nullptr;
	Label* livesLabel = nullptr;
};
