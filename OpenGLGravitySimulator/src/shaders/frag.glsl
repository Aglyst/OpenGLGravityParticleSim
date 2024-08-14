#version 330 core

out vec4 FragColor;

in float Vel;

vec3 calcParticleCol()
{
    return mix(vec3(0.0510, 1.0, 0), vec3(1.0, 0, 0), Vel * 100);
}

void main()
{
    FragColor = vec4(calcParticleCol(), 1.0f);
}