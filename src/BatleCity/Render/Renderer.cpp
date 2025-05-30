#include <BatleCity/Render/Renderer.h>

#include <iostream>

namespace RenderEngine {
bool Renderer::Init() {
  if (!gladLoadGL()) {
    std::cout << "ERROR: Can't load GLAD" << std::endl;
    return false;
  }
  return true;
}

void Renderer::DrawElements(const unsigned int primitive_type,
                            const VertexArray& vertex_array,
                            const ElementBuffer& element_buffer,
                            const ShaderProgram& shader_program) {
  shader_program.Use();
  vertex_array.bind();
  glDrawElements(primitive_type, element_buffer.getCount(), GL_UNSIGNED_INT,
                 nullptr);
}

void Renderer::SetViewport(const unsigned int width, const unsigned int height,
                           const unsigned int left_offset,
                           const unsigned int bottom_offset) {
  glViewport(left_offset, bottom_offset, width, height);
}

void Renderer::SetDepthTest(const bool enable) {
  if (enable)
    glEnable(GL_DEPTH_TEST);
  else
    glDisable(GL_DEPTH_TEST);
}

void Renderer::SetClearColor(const float r, const float g, const float b,
                             const float a) {
  glClearColor(r, g, b, a);
}

void Renderer::Clear(const unsigned int buffer_type) { glClear(buffer_type); }

std::string Renderer::GetStringOpenGL(const unsigned int name) {
  return (char*)glGetString(name);
}
}  // namespace RenderEngine