#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include "Render/ShaderProgram.h"
#include "Resources/ResourceManager.h"
#include "Render/Texture2D.h"
#include "Render/Sprite2D.h"

#include <cmath>
#include <vector>

// GLOBAL VARIEBLES

glm::ivec2 G_WINDOW_SIZE(1024, 720);

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    G_WINDOW_SIZE.x = width;
    G_WINDOW_SIZE.y = height;
    glViewport(0, 0, G_WINDOW_SIZE.x, G_WINDOW_SIZE.y);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
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

    glClearColor(0.9, 0.9, 0.9, 1); 

    {
        // RESUORVE MANAGER
        Resources::ResourceManager resource_manager(argv[0]);

        std::shared_ptr<Renderer::ShaderProgram> shader_program = resource_manager.loadShaderProrgam("Shader_program_1",
                                                                                                     "res/shaders/vertex.txt",
                                                                                                     "res/shaders/fragment.txt");
        if (!shader_program->isCompiled())
        {
            std::cerr << "Can't compiled shader program" << std::endl;
            return -1;
        }

        std::shared_ptr<Renderer::ShaderProgram> shader_program_sprite = resource_manager.loadShaderProrgam("Shader_program_2",
                                                                                                            "res/shaders/vSprite.txt",
                                                                                                            "res/shaders/fSprite.txt");
        if (!shader_program_sprite->isCompiled())
        {
            std::cerr << "Can't compiled shader program" << std::endl;
            return -1;
        }

        std::shared_ptr<Renderer::Texture2D> texture_1 = resource_manager.loadTexture("Texture_1", "res/textures/mylove.jpg");

        std::vector<std::string> names_subTextures = {"block", "topBlock", "bottomBlock", "leftBlock", "rightBlock", "leftTopBlock",
                                                      "rightTopBlock", "leftBottomBlock", "rightBottomBlock", "beton", "topBeton",
                                                      "bottomBeton", "leftBeton", "rightBeton", "leftTopBeton", "rightTopBeton",
                                                      "leftBottomBeton", "rightBottomBeton"};

        std::shared_ptr<Renderer::Texture2D> texture_2 = resource_manager.loadTextureAtlas("Texture_2", names_subTextures,
                                                                                           "res/textures/map_16x16.png", 16, 16);

        std::shared_ptr<Renderer::Sprite2D> sprite_1 = resource_manager.loadSprite("Sprite1", "Shader_program_2",
                                                                                   "Texture_1", 324.f, 320.f);

        std::shared_ptr<Renderer::Sprite2D> sprite_2 = resource_manager.loadSprite("Sprite2", "Shader_program_2",
                                                                                   "Texture_2", 300.f, 300.f, "beton");

        std::shared_ptr<Renderer::Sprite2D> sprite_3 = resource_manager.loadSprite("Sprite3", "Shader_program_2",
                                                                                   "Texture_2", 300.f, 300.f, "rightBottomBeton");

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pWindow))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            sprite_1->setPosition(glm::vec2(700.f, 400.f));
            //sprite_1->setRotation(glfwGetTime() * 10.f * sin(glfwGetTime()));

            sprite_3->setPosition(glm::vec2(0.f, 420.f));

            sprite_1->render();
            sprite_2->render();
            sprite_3->render();

            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}
