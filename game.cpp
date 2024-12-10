// Template, 2024 IGAD Edition
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2024

#include "precomp.h"
#include "game.h"

CollisionEventManager* colManager;

Player* player;
Wall* wall;
Item* item;

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{ 
	center = { SCRWIDTH / 2, SCRHEIGHT / 2 };

	colManager = new CollisionEventManager();

	player = new Player();
	item = new Item(16, 16);
	wall = new Wall(64, 64);

	// set ids
	player->GetCollider()->id = 1;
	wall->GetCollider()->id = 2;
	item->GetCollider()->id = 3;

	// setup collision events
	wall->GetCollider()->OnCollisionEnter = [&](const Collider*) {
		printf("Woahza, collision!\n");
	};

	item->GetCollider()->OnCollisionEnter = [&](const Collider*) {
		player->TakeDamage(1);
		colManager->RemoveCollider(item->GetCollider());
		delete item;
		item = nullptr;
	};

	// Add colliders to manager
	colManager->AddCollider(player->GetCollider());
	colManager->AddCollider(wall->GetCollider());
	colManager->AddCollider(item->GetCollider());
}

// -----------------------------------------------------------
// Main application tick function - Executed once per frame
// -----------------------------------------------------------
void Game::Tick( float deltaTime )
{
	colManager->UpdateCollisions();

	player->Update(deltaTime);
}

// -----------------------------------------------------------
// Main application fixed tick function - Executed once per interval
// -----------------------------------------------------------
void Game::FixedTick(float fixedDeltaTime)
{
	player->FixedUpdate(fixedDeltaTime);
}

// -----------------------------------------------------------
// Main application render function - Executed once per frame
// -----------------------------------------------------------
void Game::Render()
{
	screen->Clear(0);

	wall->Draw(screen);

	if (item)
		item->Draw(screen);

	player->Draw(screen);
}

// -----------------------------------------------------------
// Shutdown application and cleanup
// -----------------------------------------------------------
void Game::Shutdown()
{
	delete colManager;

	delete player;
	delete wall;

	if (item) delete item;
}