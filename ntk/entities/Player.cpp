#include "precomp.h"
#include "Player.h"

Player::Player()
{
	ResourceHolder& rh = ResourceHolder::Instance();
	rh.LoadSprite("assets/playership.png", "player", 9);

	m_sprite = rh.GetSprite("player");
	position = { 0, 0 };
}