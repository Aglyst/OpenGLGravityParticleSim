#include <iostream>
#include <vector>
#include <filesystem>
namespace fs = std::filesystem;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Particle.h"

static float SCR_WIDTH = 800;
static float SCR_HEIGHT = 800;

fs::path currentPath = fs::current_path();

//std::vector<Particle> particles;

std::vector<float> points;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
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

    #pragma region Initialize Particles
    points.push_back(-0.5f);
    points.push_back(-0.5f);
    points.push_back(0.0f);
    points.push_back(0.5f);
    points.push_back(-0.5f);
    points.push_back(0.0f);
    points.push_back(0.0f);
    points.push_back(0.5f);
    points.push_back(0.0f);

    #pragma endregion

    #pragma region VBO|VAO|Shader Setup
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Shader shader(currentPath.string() + "\\src\\shaders\\vert.glsl", currentPath.string() + "\\src\\shaders\\frag.glsl");
    #pragma endregion

    glfwSwapInterval(1);
    while (!glfwWindowShouldClose(window)) 
    {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        shader.Activate();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader.progID);

    glfwTerminate();
    return 0;
}
