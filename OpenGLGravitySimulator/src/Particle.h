#pragma once

#include <GLM/vec2.hpp>

struct Particle
{
public:
	glm::vec2 pos;
	glm::vec2 vel = glm::vec2(0);
	glm::vec2 force = glm::vec2(0);
	float mass;

	Particle(glm::vec2 position, float mass) : pos(position), mass(mass) { }
};