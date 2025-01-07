#include "precomp.h"
#include "Player.h"

#include <algorithm>

void Player::initialize()
{
	// set a unique sprite to the player
	ResourceHolder& rh = ResourceHolder::Instance();
	rh.LoadSprite("assets/playership.png", "player", 9);
	m_sprite = rh.GetSprite("player");
	setLayer(Layer::Player);
	name = "Player";

	position = { SCRWIDTH / 2.f, SCRHEIGHT / 2.f };

	// initialize input related attributes
	thrustInput = 0.f;
	angle = 0.f;

	// initialize speed attributes
	speedMod = 2.4f;
	rotationMod = 1.6f;
	speed = 100.f * speedMod;
	maxSpeed = 150.f;
	rotationSpeed = 90.f * rotationMod;

	// timing initialization
	timeSinceLastShot = 0.0f;

	// register object for collisions
	CollisionSystem::instance().registerObject(this,
		[this](const CollisionEvent& event)
		{
			this->onCollision(event);
		});

	// initialize health attributes
	lives = 3;
	maxLives = 3;

	// initialize audio
	fireSound = new Audio::Sound{ "assets/audio/laserShoot.wav", Audio::Sound::Type::Sound };
	hitSound = new Audio::Sound{ "assets/audio/player_hit.wav", Audio::Sound::Type::Sound };
	explosionSound = new Audio::Sound{ "assets/audio/player_explosion.wav", Audio::Sound::Type::Sound };

	fireSound->setVolume(0.5f);
	hitSound->setVolume(0.5f);
	explosionSound->setVolume(0.5f);
}

Player::~Player()
{
	// properly deallocate members
	CollisionSystem::instance().unregisterObject(this);

	delete fireSound;
	delete hitSound;
	delete explosionSound;
}

void Player::update()
{
	// Only update the player if they aren't dead
	if (!isDead())
	{
		GameObject::update();

		retrieveInput();
		keepInView();

		// shoot only in an interval
		timeSinceLastShot += Time::deltaTime;
		if (Input::getKeyDown(GLFW_KEY_SPACE) && timeSinceLastShot >= firingInterval)
		{
			fireSound->replay();
			fireProjectile();
			timeSinceLastShot = 0.0f;
		}

		// apply immunity time to player
		if (collisionTimer > 0.0f)
		{
			collisionTimer -= Time::deltaTime;
		}
	}
}

void Player::fixedUpdate()
{
	thrust();

	// only apply drag if no input is given
	if (thrustInput == 0.f)
		applySpaceBraking(50.f);
}

void Player::onCollision(const CollisionEvent& event)
{
	// no collisions when immune
	if (isImmune()) return;

	// handle only asteroid collisions, could be buggy
	// finding method found here: https://www.geeksforgeeks.org/stringnpos-in-c-with-examples/
	if (event.other->getName().find("asteroid") != std::string::npos)
	{
		collisionTimer = immunity;
		GameManager::instance().score->resetMultiplier();
		GameManager::instance().updateScoreMultiplierDisplay(GameManager::instance().score->getCurrentMultiplier());
		removeLife(1);

		if (lives != 0)
			hitSound->replay();
		else
			explosionSound->play();
	}
}

/** Add a life to the player. */
void Player::addLife(const int add)
{
	lives += add;
	lives = std::min(lives, maxLives);
	GameManager::instance().updateLivesDisplay();
}

/** Remove a life from the player. */
void Player::removeLife(const int sub)
{
	lives -= sub;
	lives = std::max(lives, 0);
	GameManager::instance().updateLivesDisplay();
}

void Player::reset()
{
	position = { SCRWIDTH / 2.f, SCRHEIGHT / 2.f };
	velocity = { 0.f, 0.f };
	lives = maxLives;
	GameManager::instance().updateLivesDisplay();
}

/**
 * Applies a continuous force to slow down the ship.
 * @param brakeForce The force that will be applied each frame.
 */
void Player::applySpaceBraking(float brakeForce)
{
	// continuously apply a force to the velocity of the player until they are fully stopped
	float currentSpeed = length(velocity);
	if (currentSpeed > 0.f)
	{
		// resharper refactored it to use std::min instead of a > comparison
		float reductionAmount = std::min(brakeForce * Time::fixedDeltaTime, currentSpeed);
		velocity = normalize(velocity) * (currentSpeed - reductionAmount);
	}
}

/** Retrieves the input and computes the thrust input based on direction. */
void Player::retrieveInput()
{
	float rotationValue = Input::getAxis(Input::Horizontal) * (rotationSpeed * rotationMod * Time::deltaTime);
	rotate(rotationValue);

	thrustInput = Input::getAxis(Input::Vertical) * speed;
}

/** Computes a forward direction and applies thrust to that direction. */
void Player::thrust()
{
	if (thrustInput != 0.f)
	{
		// normalize the speed to not go faster than the intended max speed
		if (length(velocity) > maxSpeed)
		{
			velocity = normalize(velocity) * maxSpeed;
		}

		// apply velocity in the direction of the angle (forward motion)
		velocity.x += std::cos(angle * (PI / 180.f)) * thrustInput * Time::fixedDeltaTime;
		velocity.y += std::sin(angle * (PI / 180.f)) * thrustInput * Time::fixedDeltaTime;
	}
}

/** Fire a projectile with the correct rotations and adds it to the game world. */
void Player::fireProjectile() const
{
	// spawn a new projectile based on the forward direction
	float2 projectileStart = position + float2{
		std::cos(angle * (PI / 180.f)) * 30.f,
		std::sin(angle * (PI / 180.f)) * 30.f
	};

	Projectile* newProjectile = new Projectile(projectileStart, angle);
	GameWorld::instance().addObject(newProjectile);
}
