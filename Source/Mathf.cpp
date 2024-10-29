#include "Mathf.h"
#include <stdlib.h>
#include <time.h>
#include <random>

float Mathf::Lerp(float a, float b, float t)
{
	//return a + t * (b - a);
	return a * (1.0f - t) + (b * t);
}

float Mathf::RandomRange(float min, float max)
{	
	//float value = static_cast<float>(rand() / RAND_MAX);
	//return min + (max - min) * value;

	std::mt19937 random(std::random_device{}());
	std::uniform_int_distribution<int> range(min, max);

	float randomPosition = static_cast<float>(range(random));

	return randomPosition;
	
}
