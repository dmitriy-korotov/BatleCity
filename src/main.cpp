#include <cstdlib>

#include <BatleCity/Resources/ResourceManager.h>

#include <BatleCity/Render/Renderer.h>

#include <BatleCity/Physics/PhysicsEngine.h>

#include <BatleCity/System/Window.h>

#include <BatleCity/Game/Game.h>

#include <chrono>
#include <iostream>
#include <memory>

using Clock = std::chrono::high_resolution_clock;

static double Diff(Clock::time_point finish, Clock::time_point start) {
  return std::chrono::duration_cast<std::chrono::milliseconds>(finish - start)
      .count();
}

static void WindowResizeCallback(GLFWwindow* pWindow, int width, int height) {
  const float game_aspect_ratio =
      static_cast<float>(BatleCity::Game::Instance().GetCurrentGameWidth()) /
      BatleCity::Game::Instance().GetCurrentGameHeight();

  unsigned int view_port_width = width;
  unsigned int view_port_height = height;
  unsigned int view_port_left_offset = 0;
  unsigned int view_port_bottom_offset = 0;

  if (static_cast<float>(view_port_width) / view_port_height >
      game_aspect_ratio) {
    view_port_width = view_port_height * game_aspect_ratio;
    view_port_left_offset = (width - view_port_width) / 2;
  } else {
    view_port_height = view_port_width / game_aspect_ratio;
    view_port_bottom_offset = (height - view_port_height) / 2;
  }

  BatleCity::Game::Instance().GetRenderer()->SetViewport(
      view_port_width, view_port_height, view_port_left_offset,
      view_port_bottom_offset);
}

static void KeyCallback(GLFWwindow* pWindow, int key, int scancode, int action,
                        int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(pWindow, GL_TRUE);
  }
  BatleCity::Game::Instance().SetKey(key, action);
}

int main(const int argc, const char** argv) {
  constexpr auto kWindowWidth = 13 * 16;
  constexpr auto kWindowHeight = 14 * 16;
  constexpr auto kWindowTitle = "BatleCity";

  const auto window = std::make_shared<System::Window>(
      kWindowWidth, kWindowHeight, kWindowTitle);
  if (!window->IsCreated()) {
    std::cout << "ERROR: Window creating failed" << std::endl;
    return EXIT_FAILURE;
  }

  window->SetResizeCallBack(WindowResizeCallback);
  window->SetKeyCallBack(KeyCallback);

  window->MakeContextCurrent();

  BatleCity::Game::Instance().GetResourcesManager()->SetExecutablePath(argv[0]);

  if (!BatleCity::Game::Instance().StartOn(window)) {
    std::cerr << "ERROR: Can't start game" << std::endl;
    return EXIT_FAILURE;
  }
  window->SetSize(3 * BatleCity::Game::Instance().GetCurrentGameWidth(),
                  3 * BatleCity::Game::Instance().GetCurrentGameHeight());

  auto prevFrameTime = Clock::now();

  while (!window->ShouldClose()) {
    auto currFrameTime = Clock::now();
    double duration = Diff(currFrameTime, prevFrameTime);
    prevFrameTime = currFrameTime;

    BatleCity::Game::Instance().Update(duration);
    BatleCity::Game::Instance().Render();

    window->SwapBuffers();
    window->PollEvents();
  }

  BatleCity::Game::Instance().Finish();

  return EXIT_SUCCESS;
}
