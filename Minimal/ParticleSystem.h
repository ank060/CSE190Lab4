#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

struct Particle {
	glm::vec3 position;
	glm::vec3 velocity;
	int age;
};

class ParticleSystem {
	std::vector<Particle> particles;

public:
	int maxAge = 100;

	ParticleSystem() {}
	~ParticleSystem() {}

	void spawnHitParticles(glm::vec3& position);
	void spawnParticle(glm::vec3& position, glm::vec3& velocity, int age);
	std::vector<Particle>& getParticles();

	void update();
};

#endif
