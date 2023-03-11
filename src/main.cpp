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
glm::ivec2 G_WINDOW_SIZE(13 * 16, 14 * 16);

BatleCity::Game g_game(G_WINDOW_SIZE);

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    G_WINDOW_SIZE.x = width;
    G_WINDOW_SIZE.y = height;

    const float map_aspect_ratio = 13.f / 14.f;

    unsigned int view_port_width = width;
    unsigned int view_port_height = height;
    unsigned int view_port_left_offset = 0;
    unsigned int view_port_bottom_offset = 0;

    if (static_cast<float>(view_port_width) / view_port_height > map_aspect_ratio)
    {
        view_port_width = view_port_height * map_aspect_ratio;
        view_port_left_offset = (G_WINDOW_SIZE.x - view_port_width) / 2;
    }
    else
    {
        view_port_height = view_port_width / map_aspect_ratio;
        view_port_bottom_offset = (G_WINDOW_SIZE.y - view_port_height) / 2;
    }

    RenderEngine::Renderer::setViewport(view_port_width, view_port_height, view_port_left_offset, view_port_bottom_offset);
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

    std::cout << "Render: " << RenderEngine::Renderer::getStringOpenGL(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << RenderEngine::Renderer::getStringOpenGL(GL_VERSION) << "\n\n";

    {
        Resources::ResourceManager::setExecutablePath(argv[0]);

        if (!g_game.init())
        {
            std::cerr << "Can't inital game" << std::endl;
            return -1;
        }

        auto last_time = std::chrono::high_resolution_clock::now();

        RenderEngine::Renderer::setClearColor(0.f, 0.f, 0.f);

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
