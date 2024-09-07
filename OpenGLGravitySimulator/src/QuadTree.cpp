#include "QuadTree.h"

QuadTree::QuadTree()
{
	head = new QTNode{};
}

QuadTree::~QuadTree()
{
	// TODO: Implement destructor as QT lifetime is per frame
	DeleteRecurse(head);
}

void QuadTree::Add(Particle& particle)
{
	TraverseAdd(*(head), particle, glm::vec2(0, 0), gridWidth);
}

void QuadTree::GenerateTree(std::vector<Particle> particles)
{
	float minXY = FLT_MAX;
	float maxXY = -minXY;
	
	for (Particle& p : particles) {
		float currMax, currMin;
		if (p.pos.x > p.pos.y) {
			currMax = p.pos.x;
			currMin = p.pos.y;
		}
		else {
			currMax = p.pos.y;
			currMin = p.pos.x;
		}

		if (currMax > maxXY) {
			maxXY = currMax;
		}

		if (currMin < minXY) {
			minXY = currMin;
		}

		Add(p);
	}
	gridWidth = maxXY - minXY;
}

void QuadTree::Print() 
{
	PrintRecurse(head);
}

void QuadTree::TraverseAdd(QTNode& curr, Particle& newParticle, glm::vec2 midPoint, float currWidth)
{
	if (curr.particle->mass == -1.0f && curr.tL == nullptr) {	// case: leaf node
		delete curr.particle;
		curr.particle = &newParticle;

		curr.COM = newParticle.pos;
		curr.mass = newParticle.mass;
		curr.width = currWidth;
		return;
	}
	else if (curr.particle->mass != -1.0f && curr.tL == nullptr) {					// case: leaf node needs to be subdivided

		if (curr.particle->pos.x == newParticle.pos.x && curr.particle->pos.y == newParticle.pos.y) {
			return;
		}
		
		curr.tL = new QTNode();
		curr.tR = new QTNode();
		curr.bL = new QTNode();
		curr.bR = new QTNode();

		glm::vec2 newMidPoint = glm::vec2(0,0);

		// insert new particle
		int quadToAddTo = QuadrantCheck(newParticle.pos, midPoint);		// midDist means current node/box
		if (quadToAddTo == 1) {
			newMidPoint.x = midPoint.x + currWidth / 4;
			newMidPoint.y = midPoint.y + currWidth / 4;

			TraverseAdd(*(curr.tR), newParticle, newMidPoint, currWidth /2);
		}
		else if (quadToAddTo == 2) {
			newMidPoint.x = midPoint.x - currWidth / 4;
			newMidPoint.y = midPoint.y + currWidth / 4;

			TraverseAdd(*(curr.tL), newParticle, newMidPoint, currWidth /2);
		}
		else if (quadToAddTo == 3) {
			newMidPoint.x = midPoint.x - currWidth / 4;
			newMidPoint.y = midPoint.y - currWidth / 4;

			TraverseAdd(*(curr.bL), newParticle, newMidPoint, currWidth /2);
		}
		else if (quadToAddTo == 4) {
			newMidPoint.x = midPoint.x + currWidth / 4;
			newMidPoint.y = midPoint.y - currWidth / 4;

			TraverseAdd(*(curr.bR), newParticle, newMidPoint, currWidth /2);
		}

		// insert already existing particle into quadrant
		quadToAddTo = QuadrantCheck(curr.particle->pos, midPoint);
		if (quadToAddTo == 1) {
			newMidPoint.x = midPoint.x + currWidth / 4;
			newMidPoint.y = midPoint.y + currWidth / 4;

			TraverseAdd(*(curr.tR), *(curr.particle), newMidPoint, currWidth / 2);
		}
		else if (quadToAddTo == 2) {
			newMidPoint.x = midPoint.x - currWidth / 4;
			newMidPoint.y = midPoint.y + currWidth / 4;

			TraverseAdd(*(curr.tL), *(curr.particle), newMidPoint, currWidth / 2);
		}
		else if (quadToAddTo == 3) {
			newMidPoint.x = midPoint.x - currWidth / 4;
			newMidPoint.y = midPoint.y - currWidth / 4;

			TraverseAdd(*(curr.bL), *(curr.particle), newMidPoint, currWidth / 2);
		}
		else if (quadToAddTo == 4) {
			newMidPoint.x = midPoint.x + currWidth / 4;
			newMidPoint.y = midPoint.y - currWidth / 4;

			TraverseAdd(*(curr.bR), *(curr.particle), newMidPoint, currWidth / 2);
		}

		// curr.particle == nullptr;	// doesn't work for some reason

		curr.particle = new Particle(glm::vec2(0, 0), -1);
	}
	else if (curr.particle->mass == -1.0f && curr.tL != nullptr) {					// case: node has been subdivided
		glm::vec2 newMidPoint = glm::vec2(0, 0);

		// insert new particle
		int quadToAddTo = QuadrantCheck(newParticle.pos, midPoint);		// midDist means current node/box
		if (quadToAddTo == 1) {
			newMidPoint.x = midPoint.x + currWidth / 4;
			newMidPoint.y = midPoint.y + currWidth / 4;

			TraverseAdd(*(curr.tR), newParticle, newMidPoint, currWidth / 2);
		}
		else if (quadToAddTo == 2) {
			newMidPoint.x = midPoint.x - currWidth / 4;
			newMidPoint.y = midPoint.y + currWidth / 4;

			TraverseAdd(*(curr.tL), newParticle, newMidPoint, currWidth / 2);
		}
		else if (quadToAddTo == 3) {
			newMidPoint.x = midPoint.x - currWidth / 4;
			newMidPoint.y = midPoint.y - currWidth / 4;

			TraverseAdd(*(curr.bL), newParticle, newMidPoint, currWidth / 2);
		}
		else if (quadToAddTo == 4) {
			newMidPoint.x = midPoint.x + currWidth / 4;
			newMidPoint.y = midPoint.y - currWidth / 4;

			TraverseAdd(*(curr.bR), newParticle, newMidPoint, currWidth / 2);
		}
	}

	curr.mass = curr.tL->mass + curr.tR->mass + curr.bL->mass + curr.bR->mass;
	curr.COM.x =((curr.tL->COM.x * curr.tL->mass) +
				 (curr.tR->COM.x * curr.tR->mass) +
				 (curr.bL->COM.x * curr.bL->mass) +
				 (curr.bR->COM.x * curr.bR->mass)) / (curr.mass);

	curr.COM.y = ((curr.tL->COM.y * curr.tL->mass) +
				  (curr.tR->COM.y * curr.tR->mass) +
				  (curr.bL->COM.y * curr.bL->mass) +
				  (curr.bR->COM.y * curr.bR->mass)) / (curr.mass);
}

int QuadTree::QuadrantCheck(glm::vec2 pos, glm::vec2 midPoint) {			

	if (pos.x > midPoint.x && pos.y > midPoint.y) {
		return 1;
	}
	else if (pos.x <= midPoint.x && pos.y > midPoint.y) {
		return 2;
	}
	else if (pos.x <= midPoint.x && pos.y <= midPoint.y) {
		return 3;
	}
	else if (pos.x > midPoint.x && pos.y <= midPoint.y) {
		return 4;
	}

	return -1;
}

void QuadTree::PrintRecurse(QTNode* node)
{
	if (node == nullptr) {
		return;
	}

	if (node->particle->mass != -1.0f) {
		std::cout << "(" << node->particle->pos.x << ", " << node->particle->pos.y << ")" << " | W:" << node->width << std::endl;
	}
	else {
		PrintRecurse(node->tL);
		PrintRecurse(node->tR);
		PrintRecurse(node->bL);
		PrintRecurse(node->bR);
	}
}

void QuadTree::DeleteRecurse(QTNode* node) {
	if (node == nullptr) {
		return;
	}

	if (node->particle->mass != -1.0f) {
		delete node;
	}
	else {
		DeleteRecurse(node->tL);
		DeleteRecurse(node->tR);
		DeleteRecurse(node->bL);
		DeleteRecurse(node->bR);
		delete node->particle;
		delete node;
	}
}
