// Template, 2024 IGAD Edition
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2024

#include "precomp.h"
#include "game.h"

CollisionEventManager* colManager;

Player* player;
GameObject* object;

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{ 
	center = { SCRWIDTH / 2, SCRHEIGHT / 2 };

	colManager = new CollisionEventManager();

	player = new Player();
	object = new GameObject();

	// set ids
	player->GetCollider()->id = 1;
	object->GetCollider()->id = 2;

	// setup events
	player->GetCollider()->OnCollisionEnter = [](const Collider* other) { player->OnCollisionEnter(other); };
	player->GetCollider()->OnCollisionStay = [](const Collider* other) { player->OnCollisionStay(other); };
	player->GetCollider()->OnCollisionExit = [](const Collider* other) { player->OnCollisionExit(other); };

	// setup collision events
	colManager->AddCollider(player->GetCollider());
	colManager->AddCollider(object->GetCollider());
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
// Main application render function - Executed once per frame
// -----------------------------------------------------------
void Game::Render()
{
	screen->Clear(0);

	object->Draw(screen);
	player->Draw(screen);
}

// -----------------------------------------------------------
// Shutdown application and cleanup
// -----------------------------------------------------------
void Game::Shutdown()
{
	delete colManager;

	delete object;
	delete player;
}