#include <iostream>
#include <vector>
#include <filesystem>
namespace fs = std::filesystem;
#include <chrono>
using namespace std::chrono;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/random.hpp>

#include "Shader.h"
#include "Particle.h"
#include "QuadTree.h"


static float SCR_WIDTH = 800;
static float SCR_HEIGHT = 800;

const float G = 0.000000000066743f;

float dt = 0.0001;
float oldTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

std::vector<Particle> particles;
float theta = 0.5;


fs::path currentPath = fs::current_path();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


void CalculateNaive() 
{
    for (Particle& currentP : particles) 
    {
        for (Particle& p : particles) 
        {
            if (currentP.pos != p.pos) 
            {
                float dist = glm::max(glm::distance(p.pos, currentP.pos), 0.000001f);

                glm::vec2 dir = glm::normalize(p.pos - currentP.pos);   // p.pos - currentP.pos;
                currentP.force += (G * (currentP.mass*p.mass)/(dist /* * dist*/)) * dir;
            }
        }

        glm::vec2 accel = currentP.force / currentP.mass;
        currentP.vel += accel * dt;
        currentP.pos += (accel / 2.0f * dt * dt) + (currentP.vel * dt);
    }
}

#pragma region Calculate via Barnes-Hut
void FindForce(Particle& originalParticle, QTNode* node) {
    if (node == nullptr) {
        return;
    }
    // TODO: implement pairwise force solving
    if (node->particle->mass != -1.0f) {
        // if node contains a particle
        float dist = glm::max(glm::distance(node->particle->pos, originalParticle.pos), 0.000001f);
        glm::vec2 dir = glm::normalize(node->particle->pos - originalParticle.pos);

        originalParticle.force += (G * (originalParticle.mass * node->particle->mass) / (dist)) * dir;
        return;
    }

    float quotient = (node->width) / (glm::distance(originalParticle.pos, node->COM));  // s/d
    if (quotient > theta) {
        FindForce(originalParticle, node->tL);
        FindForce(originalParticle, node->tR);
        FindForce(originalParticle, node->bL);
        FindForce(originalParticle, node->bR);
        return;
    }
    else {
        float dist = glm::max(glm::distance(node->COM, originalParticle.pos), 0.000001f);
        glm::vec2 dir = glm::normalize(node->COM - originalParticle.pos);

        originalParticle.force += (G * (originalParticle.mass * node->mass) / (dist)) * dir;
        return;
    }
}

void CalculateBarnesHut() {
    QuadTree qt;
    qt.GenerateTree(particles);

    for (Particle& p : particles)
    {
        FindForce(p, qt.head);

        glm::vec2 accel = p.force / p.mass;
        p.vel += accel * dt;
        p.pos += (accel / 2.0f * dt * dt) + (p.vel * dt);
    }
}
#pragma endregion

int main()
{
    int particleCount;

    std::cout << "Particle Count: ";
    std::cin >> particleCount;

    #pragma region Setup
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "N-Body Grav Sim", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glfwSetMouseButtonCallback(window, mouse_b

    gladLoadGL();
    #pragma endregion

    srand(time(0));
    #pragma region Initialize Particles
    for (int i = 0; i < particleCount-1; i++)
    {
        glm::vec2 pos = glm::linearRand(glm::vec2(-1.0f, -1.0f), glm::vec2(1.0, 1.0));
        particles.push_back(Particle(pos, 5.0f));
        // std::cout << "(" << pos.x << ", " << pos.y << ")" << std::endl;
    }
    particles.push_back(Particle(glm::vec2(0.75,0.75), 30.0f));
    particles.push_back(Particle(glm::vec2(0,0), 750.0f));
    //particles.push_back(Particle(glm::vec2(0.75f, 0), 30.0f));
    #pragma endregion

    #pragma region VBO|VAO Setup
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * particles.size(), &particles[0], GL_STREAM_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, mass));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, vel));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    #pragma endregion

    Shader shader(currentPath.string() + "\\src\\shaders\\vert.glsl", currentPath.string() + "\\src\\shaders\\frag.glsl");
    shader.Activate();
    shader.SetFloat("rangeMin", 1.0f);
    shader.SetFloat("rangeMax", 1000.0f);

    glEnable(GL_PROGRAM_POINT_SIZE);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glfwSwapInterval(1);
    while (!glfwWindowShouldClose(window)) 
    {
        float currentTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        dt = currentTime-oldTime + 0.1;
        oldTime = currentTime;

        // CalculateNaive();
        CalculateBarnesHut();

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Particle) * particles.size(), &particles[0]);

        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, particles.size());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader.progID);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}