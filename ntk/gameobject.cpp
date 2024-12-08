#include "precomp.h"
#include "gameobject.h"

GameObject::GameObject()
:collider(nullptr)
{
	ResourceHolder& rh = ResourceHolder::Instance();

	// Create a custom surface
	Surface* surface = rh.CreateSurface("square", 128, 128);
	surface->Bar(0, 0, 128, 128, 0xFFFFFF);

	// Create a sprite from the surface
	sprite = rh.CreateSprite("square", surface, 1);

	// Initialize collider based on sprite dimensions
	InitializeCollider();

	// Default position at screen center
	position = { SCRWIDTH / 2, SCRHEIGHT / 2 };

	// Update collider position to match object position
	UpdateColliderPosition();
}

/*
 * Update is called once per tick. 
 * @param deltaTime The deltaTime of the application running.
*/
void GameObject::Update(float)
{ }

GameObject::~GameObject()
{
	/** Delete any remaining resources. (Resource Holder manages the sprite) */
}

/**	
 * Draw the GameObject to the screen.
 * @param screen The surface it should render to.
*/
void GameObject::Draw(Surface* screen)
{
	// Draw the collider only if it exists and is in debug mode
#if _DEBUG
	if (collider)
		collider->Render(screen);
#endif

	// Draw the sprite only if it exists
	if (sprite)
		sprite->Draw(screen, (int)position.x, (int)position.y);
}

/**
 * Set the new position of the Game Object.
 * @param newPos The new position that should be set.
*/
void GameObject::SetPosition(float2 newPos)
{
	position = newPos;
}

/**
 * Called when collision first begins with another object.
 * @param other The object being collided with
 */
void GameObject::OnCollisionEnter(const Collider*)
{ }

/**
 * Called continuously while objects remain in contact.
 * @param other The object currently colliding
 */
void GameObject::OnCollisionStay(const Collider*)
{ }

/**
 * Called when objects separate after a collision.
 * @param other The object that was previously colliding
 */
void GameObject::OnCollisionExit(const Collider*)
{ }

/** 
 * @brief Centers the origin of the sprite. 
*/
void GameObject::CenterOrigin()
{
	// Only center it if the sprite exists to prevent null references
	if (sprite)
	{
		sprite->SetOrigin(sprite->GetWidth() * 0.5f, sprite->GetHeight() * 0.5f);
	}
}

/** 
 * @brief Calculates a bounding box based on the sprite size. 
*/
void GameObject::InitializeCollider()
{
	float3 bmin = { 0.0f, 0.0f, 0.0f };
	float3 bmax = {
		static_cast<float>(sprite->GetWidth()),
		static_cast<float>(sprite->GetHeight()),
		0.0f
	};

	// Create collider with these bounds
	collider = new Collider(bmin, bmax);
}

/** 
 * @brief Updates the collider's position to match object's current position.
*/
void GameObject::UpdateColliderPosition()
{
	if (collider)
	{
		// take the half width and height
		float halfWidth = sprite->GetWidth() / 2.0f;
		float halfHeight = sprite->GetHeight() / 2.0f;

		// set bounds according to the half width and height accordingly
		collider->bounds.bmin[0] = position.x - halfWidth;
		collider->bounds.bmin[1] = position.y - halfHeight;
		collider->bounds.bmin[2] = 0.0f;

		collider->bounds.bmax[0] = position.x + halfWidth;
		collider->bounds.bmax[1] = position.y + halfHeight;
		collider->bounds.bmax[2] = 0.0f;
	}
}

/** 
 * @brief Clamps the speed so it wouldn't over accelerate, this also automatically updates the position. 
*/
void GameObject::ClampSpeed(float deltaTime)
{
	// Get the magnitude of the current velocity
	float currentSpeed = std::sqrt(
		velocity.x * velocity.x +
		velocity.y * velocity.y
	);

	// Check if the currentSpeed is higher than the maxSpeed if so clamp it
	if (currentSpeed > maxSpeed)
	{
		velocity.x *= (maxSpeed / currentSpeed);
		velocity.y *= (maxSpeed / currentSpeed);
	}

	// Update position based on velocity
	position += velocity * deltaTime;
}

/**
 * @brief Applies a continous drag to gain a better control of the Game Object.
*/
void GameObject::ApplyDrag(float deltaTime)
{

}
