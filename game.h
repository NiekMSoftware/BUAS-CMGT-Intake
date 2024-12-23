// Template, 2024 IGAD Edition
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2024

#pragma once

namespace Tmpl8
{
	class Game : public TheApp
	{
	public:
		// game flow methods
		void Init();
		void Tick();
		void FixedTick();
		void Render();
		void Shutdown();
		// input handling
		void MouseUp( int ) { /* implement if you want to detect mouse button presses */ }
		void MouseDown( int ) { /* implement if you want to detect mouse button presses */ }
		void MouseMove( int x, int y ) { mousePos.x = x, mousePos.y = y; }
		void MouseWheel( float ) { /* implement if you want to handle the mouse wheel */ }
		void KeyUp( int ) { /* implement if you want to handle keys */ }
		void KeyDown( int ) { /* implement if you want to handle keys */ }

		// data members
		int2 mousePos;

	private:
		void spawnInitialAsteroids();

	private:
		const int MAX_LARGE_ASTEROIDS = 12;

		// game tracking
		int score = 0;
	};

	// TODO: Change this to an actual class itself
	class GameManager
	{
	public:
		static GameManager instance() { static GameManager s; return s; }

		void addScore(const int v) { score += v; }
		int getScore() const { return score; }

	private:
		int score = 0;
	};
} // namespace Tmpl8