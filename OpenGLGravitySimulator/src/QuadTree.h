#pragma once

#include <vector>

#include <GLM/vec2.hpp>

#include "Particle.h"

struct QTNode {
	Particle* particle = nullptr;
	glm::vec2 COM	= glm::vec2(0,0);		// center of mass of node
	float mass		= 0;					// total mass
	QTNode* tL		= nullptr;
	QTNode* tR		= nullptr;
	QTNode* bL		= nullptr;
	QTNode* bR		= nullptr;
};

class QuadTree
{
public:
	QTNode* head;

	QuadTree();
	~QuadTree();

	void Add(Particle& particle);
	void Generate(std::vector<Particle>& particles);
private:
	void TraverseAdd(QTNode& head, Particle& particle, float midDist, float currWidth);
	int QuadrantCheck(glm::vec2 pos, float midDist);
};

