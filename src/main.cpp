#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Resources/ResourceManager.h"

#include "Render/Renderer.h"

#include "Physics/PhysicsEngine.h"

#include "System/Window.h"

#include "Game/Game.h"



#include <iostream>
#include <memory>
#include <chrono>



static glm::ivec2 G_WINDOW_SIZE(13 * 16, 14 * 16);

std::unique_ptr<BatleCity::Game> g_game = std::make_unique<BatleCity::Game>();

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    G_WINDOW_SIZE.x = width;
    G_WINDOW_SIZE.y = height;

    const float game_aspect_ratio = static_cast<float>(g_game->getCurrentGameWidth()) / g_game->getCurrentGameHeight();

    unsigned int view_port_width = G_WINDOW_SIZE.x;
    unsigned int view_port_height = G_WINDOW_SIZE.y;
    unsigned int view_port_left_offset = 0;
    unsigned int view_port_bottom_offset = 0;

    if (static_cast<float>(view_port_width) / view_port_height > game_aspect_ratio)
    {
        view_port_width = view_port_height * game_aspect_ratio;
        view_port_left_offset = (G_WINDOW_SIZE.x - view_port_width) / 2;
    }
    else
    {
        view_port_height = view_port_width / game_aspect_ratio;
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
    g_game->setKey(key, action);
}



int main(const int argc, const char** argv)
{
    if (!my_system::Window::initWindows())
    {
        std::cout << "ERROR: Windows init failed" << std::endl;
        return -1;
    }

    my_system::Window::setHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    my_system::Window::setHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    my_system::Window::setHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    std::shared_ptr<my_system::Window> window_ptr = std::make_shared<my_system::Window>(G_WINDOW_SIZE.x, G_WINDOW_SIZE.y, "BatleCity");
    if (!window_ptr->isCreated())
    {
        std::cout << "ERROR: Window creating failed" << std::endl;
        my_system::Window::terminate();
        return -1;
    }
    
    window_ptr->setResizeCallBack(glfwWindowSizeCallback);
    window_ptr->setKeyCallBack(glfwKeyCallback);

    window_ptr->makeContextCurrent();

    if (!gladLoadGL())
    {
        std::cout << "ERROR: Can't load GLAD" << std::endl;
        return -1;
    }

    std::cout << "Render: " << RenderEngine::Renderer::getStringOpenGL(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << RenderEngine::Renderer::getStringOpenGL(GL_VERSION) << "\n\n";

    {
        Resources::ResourceManager::setExecutablePath(argv[0]);
        Physics::PhysicsEngine::init();

        if (!g_game->init(window_ptr))
        {
            std::cerr << "ERROR: Can't inital game" << std::endl;
            return -1;
        }
        window_ptr->setSize(3 * static_cast<int>(g_game->getCurrentGameWidth()), 3 * static_cast<int>(g_game->getCurrentGameHeight()));
        
        auto last_time = std::chrono::high_resolution_clock::now();

        RenderEngine::Renderer::setClearColor(0.f, 0.f, 0.f);
        RenderEngine::Renderer::setDepthTest(true);



        while (!window_ptr->ShouldClose())
        {
            RenderEngine::Renderer::clear(GL_COLOR_BUFFER_BIT);
            RenderEngine::Renderer::clear(GL_DEPTH_BUFFER_BIT);

            auto current_time = std::chrono::high_resolution_clock::now();
            double duration = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_time).count() + 1;
            last_time = current_time;


            //std::cout << "FPS:\t" << static_cast<double>(1000) / duration << std::endl;

            g_game->update(duration);
            g_game->render();

            window_ptr->swapBuffers();

            window_ptr->pollEvents();
        }
    }
    
    Resources::ResourceManager::unloadAllResources();
    Physics::PhysicsEngine::terminate();

    my_system::Window::terminate();
    
    return 0;
}
