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

static float SCR_WIDTH = 800;
static float SCR_HEIGHT = 800;

const float G = 0.000000000066743;

float dt = 0.0001;
float oldTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

fs::path currentPath = fs::current_path();

std::vector<Particle> particles;
std::vector<glm::vec2> points;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void CalculateValues() 
{
    for (Particle& currentP : particles) 
    {
        for (Particle& p : particles) 
        {
            if (currentP.pos != p.pos) 
            {
                float dist = glm::distance(p.pos, currentP.pos);

                // std::cout << (-G * (currentP.mass * p.mass) / (dist * dist)) << std::endl;
                currentP.force += (float)(-G * (currentP.mass*p.mass)/(dist * dist)) * (currentP.pos-p.pos);
            }
            else 
            {
                currentP.force = glm::vec2(0.1f);
            }
        }
        glm::vec2 accel = currentP.force / currentP.mass;
        currentP.vel += accel * dt;
        currentP.pos += (accel / 2.0f * dt * dt) + (currentP.vel * dt);
    }
}

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

    gladLoadGL();

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    #pragma endregion

    srand(time(0));
    #pragma region Initialize Particles
    for (int i = 0; i < particleCount-1; i++)
    {
        glm::vec2 pos = glm::linearRand(glm::vec2(-1.0, -1.0), glm::vec2(1.0, 1.0));
        particles.push_back(Particle(pos, 100000.0f));
    }

    particles.push_back(Particle(glm::vec2(0.0f), 5000000.0f));
    #pragma endregion

    #pragma region VBO|VAO|Shader Setup
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * particles.size(), &particles[0], GL_STREAM_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2) * 3 + sizeof(GL_FLOAT), 0);
    glEnableVertexAttribArray(0);

    /*
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2) * 4 + sizeof(GL_FLOAT), (void*)offsetof(Particle, vel));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2) * 4 + sizeof(GL_FLOAT), (void*)offsetof(Particle, force));
    glEnableVertexAttribArray(2);
    */
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(glm::vec2) * 4 + sizeof(GL_FLOAT), (void*)offsetof(Particle, mass));
    glEnableVertexAttribArray(1);

    /*
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * points.size(), &points[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
    glEnableVertexAttribArray(0);
    */

    glBindVertexArray(0);


    Shader shader(currentPath.string() + "\\src\\shaders\\vert.glsl", currentPath.string() + "\\src\\shaders\\frag.glsl");
    #pragma endregion

    glPointSize(1.0f);
    // glDisable(GL_CULL_FACE);
    // glDisable(GL_DEPTH_TEST);


    glfwSwapInterval(1);
    while (!glfwWindowShouldClose(window)) 
    {
        float currentTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        dt = currentTime-oldTime + 0.1;
        oldTime = currentTime;
        CalculateValues();

        //for (Particle& p : particles) {
        //    p.pos = glm::linearRand(glm::vec2(-1.0, -1.0), glm::vec2(1.0, 1.0));
        //}
        //glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * particles.size(), &particles[0], GL_STREAM_DRAW);
        //void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        //memcpy(ptr, &particles, sizeof(sizeof(Particle) * particles.size()));
        //glUnmapBuffer(GL_ARRAY_BUFFER);
        // glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * particles.size(), NULL, GL_DYNAMIC_DRAW);

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Particle) * particles.size(), &particles[0]);

        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Activate();
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
