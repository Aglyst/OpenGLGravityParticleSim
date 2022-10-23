#version 330 core

layout (location = 0) in vec2 vPos;
layout (location = 3) in vec2 vVel;
layout (location = 2) in vec2 vForce;
layout (location = 1) in float vMass;


void main()
{
	gl_Position = vec4(vPos, 0.0f, 1.0f);
}
