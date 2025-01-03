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

	position = { SCRWIDTH / 2.f, SCRHEIGHT / 2.f };

	// initialize computable attributes
	thrustInput = 0.f;
	angle = 0.f;

	// initialize speed attributes
	speedMod = 2.4f;
	rotationMod = 1.2f;
	speed = 100.f * speedMod;
	maxSpeed = 150.f;
	rotationSpeed = 90.f * rotationMod;

	// timing initialization
	timeSinceLastShot = 0.0f;

	// register object
	name = "Player";
	CollisionSystem::instance().registerObject(this,
		[this](const CollisionEvent& event)
		{
			this->onCollision(event);
		});

	// initialize health attributes
	lives = 3;
	maxLives = 5;
}

Player::~Player()
{
	CollisionSystem::instance().unregisterObject(this);
}

void Player::update()
{
	if (!isDead())
	{
		GameObject::update();

		retrieveInput();
		keepInView();

		// shooting logic
		timeSinceLastShot += Time::deltaTime;
		if (Input::getKeyDown(GLFW_KEY_SPACE) && timeSinceLastShot >= firingInterval)
		{
			fireProjectile();
			timeSinceLastShot = 0.0f;
		}

		// immunity logic
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
	if (isImmune()) return;
	if (event.other->getName().find("asteroid") != std::string::npos)
	{
		collisionTimer = immunity;
		GameManager::instance().score->reset();
		removeLife(1);
	}
}

/** Add a life to the player. */
void Player::addLife(const int add)
{
	lives += add;
	lives = std::min(lives, maxLives);
	GameManager::instance().updateLivesDisplay(lives);
}

/** Remove a life from the player. */
void Player::removeLife(const int sub)
{
	lives -= sub;
	lives = std::max(lives, 0);
	GameManager::instance().updateLivesDisplay(lives);
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
		float reductionAmount = std::min(brakeForce * Time::fixedDeltaTime, currentSpeed);
		velocity = normalize(velocity) * (currentSpeed - reductionAmount);
	}
}

/** Retrieves the input and computes the thrust input based on direction. */
void Player::retrieveInput()
{
	float rotationValue = Input::getAxis(Input::Horizontal) * (rotationSpeed * rotationMod * Time::deltaTime);
	rotate(rotationValue);

	float thrustValue = Input::getAxis(Input::Vertical);
	if (thrustValue != 0.f)
	{
		float forwardMovement = speed * thrustValue;
		thrustInput = forwardMovement;
	}
	else
	{
		thrustInput = 0.f;
	}
}

/** Computes a forward direction and applies thrust to that direction. */
void Player::thrust()
{
	if (thrustInput != 0.f)
	{
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
	// create or instantiate projectile
	float2 projectileStart = position +
		float2(std::cos(angle * (PI / 180.f)) * 30.f,
			std::sin(angle * (PI / 180.f)) * 30.f);

	Projectile* newProjectile = new Projectile(projectileStart, angle);
	GameWorld::instance().addObject(newProjectile);
}
