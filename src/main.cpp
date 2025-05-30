#include <cstdlib>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <BatleCity/Resources/ResourceManager.h>

#include <BatleCity/Render/Renderer.h>

#include <BatleCity/Physics/PhysicsEngine.h>

#include <BatleCity/System/Window.h>

#include <BatleCity/Game/Game.h>

#include <chrono>
#include <iostream>
#include <memory>


static glm::ivec2 G_WINDOW_SIZE(13 * 16, 14 * 16);

std::unique_ptr<BatleCity::Game> g_game = std::make_unique<BatleCity::Game>();

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
  G_WINDOW_SIZE.x = width;
  G_WINDOW_SIZE.y = height;

  const float game_aspect_ratio =
      static_cast<float>(g_game->getCurrentGameWidth()) /
      g_game->getCurrentGameHeight();

  unsigned int view_port_width = G_WINDOW_SIZE.x;
  unsigned int view_port_height = G_WINDOW_SIZE.y;
  unsigned int view_port_left_offset = 0;
  unsigned int view_port_bottom_offset = 0;

  if (static_cast<float>(view_port_width) / view_port_height >
      game_aspect_ratio) {
    view_port_width = view_port_height * game_aspect_ratio;
    view_port_left_offset = (G_WINDOW_SIZE.x - view_port_width) / 2;
  } else {
    view_port_height = view_port_width / game_aspect_ratio;
    view_port_bottom_offset = (G_WINDOW_SIZE.y - view_port_height) / 2;
  }

  RenderEngine::Renderer::setViewport(view_port_width, view_port_height,
                                      view_port_left_offset,
                                      view_port_bottom_offset);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action,
                     int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(pWindow, GL_TRUE);
  }
  g_game->setKey(key, action);
}

int main(const int argc, const char** argv) {
  if (!System::Window::InitWindows()) {
    std::cout << "ERROR: Windows init failed" << std::endl;
    return EXIT_FAILURE;
  }

  std::shared_ptr<System::Window> window =
      std::make_shared<System::Window>(G_WINDOW_SIZE.x, G_WINDOW_SIZE.y,
                                          "BatleCity");
  if (!window->IsCreated()) {
    std::cout << "ERROR: Window creating failed" << std::endl;
    System::Window::Terminate();
    return EXIT_FAILURE;
  }

  window->SetResizeCallBack(glfwWindowSizeCallback);
  window->SetKeyCallBack(glfwKeyCallback);

  window->MakeContextCurrent();

  if (!gladLoadGL()) {
    std::cout << "ERROR: Can't load GLAD" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "Render: "
            << RenderEngine::Renderer::getStringOpenGL(GL_RENDERER)
            << std::endl;
  std::cout << "OpenGL version: "
            << RenderEngine::Renderer::getStringOpenGL(GL_VERSION) << "\n\n";

  {
    Resources::ResourceManager::setExecutablePath(argv[0]);
    Physics::PhysicsEngine::init();

    if (!g_game->init(window)) {
      std::cerr << "ERROR: Can't inital game" << std::endl;
      return -1;
    }
    window->SetSize(3 * static_cast<int>(g_game->getCurrentGameWidth()),
                        3 * static_cast<int>(g_game->getCurrentGameHeight()));

    auto last_time = std::chrono::high_resolution_clock::now();

    RenderEngine::Renderer::setClearColor(0.f, 0.f, 0.f);
    RenderEngine::Renderer::setDepthTest(true);

    while (!window->ShouldClose()) {
      RenderEngine::Renderer::clear(GL_COLOR_BUFFER_BIT);
      RenderEngine::Renderer::clear(GL_DEPTH_BUFFER_BIT);

      auto current_time = std::chrono::high_resolution_clock::now();
      double duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                            current_time - last_time)
                            .count() +
                        1;
      last_time = current_time;

      g_game->update(duration);
      g_game->render();

      window->SwapBuffers();

      window->PollEvents();
    }
  }

  g_game.reset();

  Resources::ResourceManager::unloadAllResources();
  Physics::PhysicsEngine::Terminate();

  System::Window::Terminate();

  return EXIT_SUCCESS;
}
