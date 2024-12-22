#include "precomp.h"
#include "Helpers.h"

float Time::deltaTime = 0.0f;
float Time::fixedDeltaTime = 0.0f;

float Random::getRandomFloat(float min, float max)
{
	// An AI assistant helped me with this random generator
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(min, max);

	return dis(gen);
}

float Random::getRandomAngle()
{
	return getRandomFloat(0.0f, 2.0f * PI);
}

float2 Random::getRandomPositionAwayFrom(const float2& point, float minDistance, float maxWidth, float maxHeight)
{
	// try up to 10 times to find a valid position
	for (int a = 0; a < 10; a++)
	{
		float2 randomPos = {
			getRandomFloat(0.0f, maxWidth),
			getRandomFloat(0.0f, maxHeight)
		};

		// Calculate distance to the point we're avoiding
		float2 diff = randomPos - point;
		float distance = length(diff);

		if (distance >= minDistance)
		{
			return randomPos;
		}
	}

	// if we failed to find a good random position, generate one at exact minimum distance
	float angle = getRandomAngle();
	return float2{ point.x + cos(angle) * minDistance, point.y + sin(angle) * minDistance };
}
