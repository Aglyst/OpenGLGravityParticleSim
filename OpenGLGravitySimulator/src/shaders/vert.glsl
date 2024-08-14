#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in float aMass;

layout (location = 2) in vec2 aVel;
layout (location = 3) in vec2 aForce;



//layout (std140) uniform Particles
//{
//	
//};

out float Vel;

uniform float rangeMin;
uniform float rangeMax;

float calcPointSize(float mass)
{
	return 1 + (99)/(rangeMax - rangeMin) * (mass - rangeMin);
}

void main()
{
	gl_PointSize = calcPointSize(aMass);
	gl_Position = vec4(aPos, 0.0f, 1.0f);
	Vel = pow(aVel.x, 2.0) + pow(aVel.y, 2.0);
}
