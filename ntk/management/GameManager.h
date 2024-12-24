#pragma once

class GameManager
{
public:
	static GameManager& instance();
	void instantiate();
	void clean();

	void setScoreLabel(Label* label) { scoreLabel = label; }
	void addScore(const int v);
	int getScore() const { return score; }

private:
	int score = 0;
	Label* scoreLabel = nullptr;
};
