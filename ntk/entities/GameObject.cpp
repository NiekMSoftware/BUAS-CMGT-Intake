#include "precomp.h"
#include "GameObject.h"

GameObject::GameObject()
{
	ResourceHolder& rh = ResourceHolder::Instance();

	velocity = { 0, 0 };
	dragCoefficient = 0.05f;
	m_sprite = rh.CreateSquare("square", 32, 32);
	position = { 100, 100 };
	angle = 0.0f;
}

GameObject::~GameObject()
{
	// [READ ME] - Sprite won't be deleted from here, the resource holder makes sure the allocated memory gets safely removed.
}

void GameObject::update(float deltaTime)
{
	position.x += velocity.x * deltaTime;
	position.y += velocity.y * deltaTime;
}

void GameObject::fixedUpdate(float fixedDeltaTime)
{
	applyDrag(fixedDeltaTime);
}

void GameObject::render(Surface* screen)
{
	//m_sprite->Draw(screen, static_cast<int>(position.x), static_cast<int>(position.y));
	m_sprite->Draw(screen, static_cast<int>(position.x), static_cast<int>(position.y), 1.f, angle);
}

float2 GameObject::getPosition() const
{
	return position;
}

void GameObject::translate(const float2& translation)
{
	position += translation;
}

void GameObject::rotate(const float& a)
{
	angle += a;
}

aabb GameObject::getCollider() const
{
	return collider;
}

Sprite* GameObject::getSprite() const
{
	return m_sprite;
}

float GameObject::calculateDragForce() const
{
	float speedSquared = length(velocity);

	float dragForce = 0.5f *
					  FLUID_DENSITY *
					  speedSquared * speedSquared * 
					  dragCoefficient;

	return dragForce;
}

void GameObject::applyDrag(float fixedDeltaTime)
{
	if (length(velocity) == 0.f) return;

	float dragForce = calculateDragForce();
	float dragAcceleration = dragForce / mass;

	// Get direction of velocity (opposite to motion)
	float2 dragDirection = normalize(velocity) * -1;

	// Apply drag force directly to each velocity component
	velocity.x += dragDirection.x * dragAcceleration * fixedDeltaTime;
	velocity.y += dragDirection.y * dragAcceleration * fixedDeltaTime;

	// Prevent velocity from being too small and stopping too slowly
	if (std::abs(velocity.x) < EPSILON) velocity.x = 0;
	if (std::abs(velocity.y) < EPSILON) velocity.y = 0;
}
