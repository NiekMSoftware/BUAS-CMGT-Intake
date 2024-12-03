#include "precomp.h"
#include "gameobject.h"

// Game object logic
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

void GameObject::Update(float)
{ }

GameObject::~GameObject()
{
	/** Delete any remaining resources. (Resource Holder manages the sprite) */
}

void GameObject::Draw(Surface* screen)
{
	if (collider)
		collider->Render(screen);
	if (sprite)
		sprite->Draw(screen, (int)position.x, (int)position.y);
}

void GameObject::SetPosition(float2 newPos)
{
	position = newPos;
}

void GameObject::OnCollisionEnter(const Collider*)
{ }

void GameObject::OnCollisionStay(const Collider*)
{ }

void GameObject::OnCollisionExit(const Collider*)
{ }

void GameObject::CenterOrigin()
{
	if (sprite)
	{
		sprite->SetOrigin(sprite->GetWidth() * 0.5f, sprite->GetHeight() * 0.5f);
	}
}

void GameObject::InitializeCollider()
{
	// Calculate bounding box based on sprite size
	float3 bmin = { 0.0f, 0.0f, 0.0f };
	float3 bmax = {
		static_cast<float>(sprite->GetWidth()),
		static_cast<float>(sprite->GetHeight()),
		0.0f
	};

	// Create collider with these bounds
	collider = new Collider(bmin, bmax);
}

void GameObject::UpdateColliderPosition()
{
	if (collider)
	{
		// Update collider position to match object's current position
		float halfWidth = sprite->GetWidth() / 2.0f;
		float halfHeight = sprite->GetHeight() / 2.0f;

		collider->bounds.bmin[0] = position.x - halfWidth;
		collider->bounds.bmin[1] = position.y - halfHeight;
		collider->bounds.bmin[2] = 0.0f;

		collider->bounds.bmax[0] = position.x + halfWidth;
		collider->bounds.bmax[1] = position.y + halfHeight;
		collider->bounds.bmax[2] = 0.0f;
	}
}