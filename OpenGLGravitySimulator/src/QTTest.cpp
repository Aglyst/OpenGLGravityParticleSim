#include <iostream>

#include "QuadTree.h"

int testQT() 
{
	QuadTree q;
	char exit = 'n';



	while (exit != 'y') {
		std::cout << "Enter (x,y): ";

		float x, y;
		scanf_s("%f,%f", &x, &y);

		Particle* p = new Particle(glm::vec2(x, y), 1);
		q.Add(*p);

		q.Print();

		std::cout << "Exit? y/n: ";
		scanf_s("%c", &exit);
	}
}