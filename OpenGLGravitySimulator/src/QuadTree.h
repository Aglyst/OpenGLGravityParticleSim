#pragma once

#include <iostream>
#include <vector>

#include <GLM/vec2.hpp>

#include "Particle.h"

struct QTNode {
	Particle* particle = new Particle(glm::vec2(0,0), -1.0f);

	glm::vec2 COM	= glm::vec2(0,0);		// center of mass of node
	float mass		= 0;					// total mass
	float width;							// width of cell(width of head will always be 2)

	QTNode* tL		= nullptr;
	QTNode* tR		= nullptr;
	QTNode* bL		= nullptr;
	QTNode* bR		= nullptr;
};

class QuadTree
{
public:
	QTNode* head;
	float gridWidth = 2.0f;

	QuadTree();
	~QuadTree();

	void Add(Particle& particle);
	void GenerateTree(std::vector<Particle> particles);
	void Print();
private:
	void TraverseAdd(QTNode& head, Particle& particle, glm::vec2 midPoint, float currWidth);
	int QuadrantCheck(glm::vec2 pos, glm::vec2 midPoint);
	void PrintRecurse(QTNode* node);
	void DeleteRecurse(QTNode* node);
};
