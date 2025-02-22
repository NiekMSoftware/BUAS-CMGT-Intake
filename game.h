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
		void handleGameState();

		void setupLabels();
		void setupGame();
		void initWorld();
		void reset();

		const int MAX_LARGE_ASTEROIDS = 12;

		Player* player = nullptr;
		Sprite* starField = nullptr;
		AsteroidPool* asteroidPool = nullptr;

		Label* scoreLabel = nullptr;
		Label* scoreMultiplierLabel = nullptr;
		Label* lifeLabel = nullptr;
		Label* waveLabel = nullptr;
		Label* clusterLabel = nullptr;
	};
} // namespace Tmpl8