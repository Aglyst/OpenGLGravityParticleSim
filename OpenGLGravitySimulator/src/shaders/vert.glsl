#version 330 core

layout (location = 0) in vec2 vPos;
layout (location = 1) in float vMass;

layout (location = 2) in vec2 vVel;
layout (location = 3) in vec2 vForce;



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
	gl_PointSize = calcPointSize(vMass);
	gl_Position = vec4(vPos, 0.0f, 1.0f);
	Vel = pow(vVel.x, 2.0) + pow(vVel.y, 2.0);
}
