#pragma once

#include <GLM/vec2.hpp>
#include <GLM/vec3.hpp>

struct Particle
{
	glm::vec2 pos;
	glm::vec2 vel;
	float mass;
};