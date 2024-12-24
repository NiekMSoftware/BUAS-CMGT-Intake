#pragma once

class Player;

enum class GameState
{
	InMenu,
	Playing,
	Paused,
	GameOver
};

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

	void setCurrentState(GameState newState) { currentState = newState; }
	GameState getCurrentState() const { return currentState; }

private:
	int score = 0;

	// Ui components
	Label* scoreLabel = nullptr;
	Label* livesLabel = nullptr;

	// state management
	GameState currentState = GameState::Playing;
};
