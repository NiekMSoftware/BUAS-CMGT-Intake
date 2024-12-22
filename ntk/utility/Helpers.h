#pragma once

/** A more accessible structure to retrieve deltaTimes and use them. */
struct Time
{
	static float deltaTime;
	static float fixedDeltaTime;
};

struct Random
{
	static float getRandomFloat(float min, float max);
	static float getRandomFloatClamped(float min, float max);
	static float getRandomAngle();
	static float2 getRandomPositionAwayFrom(const float2& point, float minDistance, float maxWidth, float maxHeight);
};