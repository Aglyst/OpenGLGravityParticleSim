#include "QuadTree.h"

QuadTree::QuadTree()
{
	head = new QTNode{};
}

QuadTree::~QuadTree()
{

}

void QuadTree::Add(Particle& particle)
{
	
}

void QuadTree::Generate(std::vector<Particle>& particles)
{
	
}

void QuadTree::TraverseAdd(QTNode& curr, Particle& particle, float midDist, float currWidth)
{
	if (curr.particle == nullptr && curr.tL == nullptr) {	// case: leaf node
		curr.particle = &particle;
		curr.COM = particle.pos;
		curr.mass = particle.mass;
		return;
	}
	else if (curr.particle != nullptr && curr.tL == nullptr) {					// case: leaf node needs to be subdivided
		curr.tL = new QTNode();
		curr.tR = new QTNode();
		curr.bL = new QTNode();
		curr.bR = new QTNode();

		float cachedMidDist = midDist;

		// insert new particle
		int quadToAddTo = QuadrantCheck(particle.pos, midDist);		// midDist means current node/box
		if (quadToAddTo == 1) {
			midDist = midDist + currWidth / 4;							// midDist means node we're traversing into 
																		// ToDo: figure out how to calculate midDist for next node and account for negatives
			TraverseAdd(*(curr.tR), particle, midDist, currWidth /2);
		}
		else if (quadToAddTo == 2) {
			midDist = ;
			TraverseAdd(*(curr.tL), particle, midDist, currWidth /2);
		}
		else if (quadToAddTo == 3) {
			midDist = ;
			TraverseAdd(*(curr.bL), particle, midDist, currWidth /2);
		}
		else if (quadToAddTo == 4) {
			midDist = ;
			TraverseAdd(*(curr.bR), particle, midDist, currWidth /2);
		}

		midDist = cachedMidDist;

		// insert already existing particle into quadrant
		quadToAddTo = QuadrantCheck(curr.particle->pos, midDist);
		if (quadToAddTo == 1) {
			midDist = midDist + currWidth / 4;							
			TraverseAdd(*(curr.tR), particle, midDist, currWidth / 2);
		}
		else if (quadToAddTo == 2) {
			midDist = ;
			TraverseAdd(*(curr.tL), particle, midDist, currWidth / 2);
		}
		else if (quadToAddTo == 3) {
			midDist = ;
			TraverseAdd(*(curr.bL), particle, midDist, currWidth / 2);
		}
		else if (quadToAddTo == 4) {
			midDist = ;
			TraverseAdd(*(curr.bR), particle, midDist, currWidth / 2);
		}

		curr.particle == nullptr;	// particle moved to quadrant

		//if (particle.pos.x <= midDist && particle.pos.y <= midDist) {
		//	TraverseAdd(*(curr.bL), particle, midDist);
		//}
		//else if (particle.pos.x <= midDist && particle.pos.y > midDist) {
		//	TraverseAdd(*(curr.tL), particle, midDist);
		//}
		//else if (particle.pos.x > midDist && particle.pos.y <= midDist) {
		//	TraverseAdd(*(curr.bR), particle, midDist);
		//}
		//else if (particle.pos.x > midDist && particle.pos.y > midDist) {
		//	TraverseAdd(*(curr.tR), particle, midDist);
		//}
	}
	else if (curr.particle == nullptr && curr.tL != nullptr) {					// case: node has been subdivided

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



	// 
}

int QuadTree::QuadrantCheck(glm::vec2 pos, float midDist) {			

	if (pos.x > midDist && pos.y > midDist) {
		return 1;
	}
	else if (pos.x <= midDist && pos.y > midDist) {
		return 2;
	}
	else if (pos.x <= midDist && pos.y <= midDist) {
		return 3;
	}
	else if (pos.x > midDist && pos.y <= midDist) {
		return 4;
	}

	return -1;
}
