#pragma once
#include <random>

class RandomGenerator {
public:
	float min;
	float max;

	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution;

	RandomGenerator() {
		distribution = std::uniform_real_distribution<float> (0.0f, 1.0f);
	}

	/*RandomGenerator(float min, float max)
	{
		this->min = min;
		this->max = max;
		distribution = std::uniform_real_distribution<float>(min, max);
	}*/

	float generate()
	{
		return distribution(generator);
	}

	float generate(float min, float max)
	{
		return distribution(generator) * (max - min) + min;
	}

};

static std::default_random_engine generator;
static std::uniform_real_distribution<float> distribution(0, 1);

static float randf() // Generates random number between 0 and 1
{
	return distribution(generator);
}

static float randf(float start, float end)
{
	return distribution(generator) * abs(end - start) + std::min(start, end);
}