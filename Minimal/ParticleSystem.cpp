#include "ParticleSystem.h"

const float GRAVITY = 0.0002;
const float VELOCITY = 0.005;

float getRandomRange(float range)
{
	return (((rand() % 10) - 5) / 5.0F) * range;
}

glm::vec3 getRandomVector(float range)
{
	return glm::vec3(getRandomRange(range), getRandomRange(range) + 0.01, getRandomRange(range));
}

void ParticleSystem::spawnHitParticles(glm::vec3& position)
{
	int count = (rand() % 15) + 20;
	for (auto it = particles.begin(), et = particles.end(); it != et; ++it)
	{
		Particle& particle = *it;
		if (particle.age <= 0)
		{
			particle.position = position;
			particle.velocity = getRandomVector(VELOCITY);
			particle.age = maxAge;
			--count;
			if (count <= 0) return;
		}
	}

	for (; count >= 0; --count)
	{
		spawnParticle(position, getRandomVector(VELOCITY), maxAge);
	}
}

void ParticleSystem::spawnParticle(glm::vec3& position, glm::vec3& velocity, int age)
{
	particles.push_back(Particle{position, velocity, age});
}

std::vector<Particle>& ParticleSystem::getParticles()
{
	return particles;
}

void ParticleSystem::update()
{
	for (auto it = particles.begin(), et = particles.end(); it != et; ++it)
	{
		Particle& particle = *it;
		if (particle.age > 0)
		{
			particle.position += particle.velocity;
			particle.velocity.y -= GRAVITY;
			particle.age--;
		}
	}
}