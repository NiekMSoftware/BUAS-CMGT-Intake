#pragma once

class Player;

class GameManager
{
public:
	static GameManager& instance();
	void instantiate();
	void clean();

	void setScoreLabel(Label* label) { scoreLabel = label; }
	void addScore(int v);
	int getScore() const { return score; }

	void setLivesLabel(Label* label) { livesLabel = label; }
	void updateLivesDisplay(int currentLives);
private:
	int score = 0;
	Label* scoreLabel = nullptr;

	Label* livesLabel = nullptr;
};
