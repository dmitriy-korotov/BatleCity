#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include "Render/ShaderProgram.h"
#include "Resources/ResourceManager.h"
#include "Render/Texture2D.h"
#include "Render/Sprite2D.h"
#include "Render/AnimatedSprite2D.h"
#include "Game/Game.h"
#include "Render/Renderer.h"

#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>


// GLOBAL VARIEBLES
glm::ivec2 G_WINDOW_SIZE(1024, 720);

BatleCity::Game g_game(G_WINDOW_SIZE);

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    G_WINDOW_SIZE.x = width;
    G_WINDOW_SIZE.y = height;
    RenderEngine::Renderer::setViewport(width, height);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
    g_game.setKey(key, action);
}

int main(const int argc, const char** argv)
{
    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "glfwInit failed" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* pWindow = glfwCreateWindow(G_WINDOW_SIZE.x, G_WINDOW_SIZE.y, "GameOpenGL", nullptr, nullptr);
    if (!pWindow)
    {
        std::cout << "glfwCreateWindow failed" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Callbacks
    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGL())
    {
        std::cout << "Can't load GLAD" << std::endl;
        return -1;
    }

    std::cout << "Render: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    {
        Resources::ResourceManager::setExecutablePath(argv[0]);

        g_game.init();

        auto last_time = std::chrono::high_resolution_clock::now();

        RenderEngine::Renderer::setClearColor(0, 0, 0);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pWindow))
        {
            /* Render here */
            RenderEngine::Renderer::clear(GL_COLOR_BUFFER_BIT);

            auto current_time = std::chrono::high_resolution_clock::now();
            uint64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time - last_time).count();
            last_time = current_time;

            g_game.update(duration);
            g_game.render();

            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    Resources::ResourceManager::unloadAllResources();
    glfwTerminate();
    return 0;
}
