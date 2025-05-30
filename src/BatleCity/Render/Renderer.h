#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include <BatleCity/Render/ElementBuffer.h>
#include <BatleCity/Render/ShaderProgram.h>
#include <BatleCity/Render/VertexArray.h>

namespace RenderEngine {
class Renderer final {
 public:
  bool Init();

  void DrawElements(const unsigned int primitive_type,
                    const VertexArray& vertex_array,
                    const ElementBuffer& element_buffer,
                    const ShaderProgram& shader_program);
  void SetClearColor(const float r, const float g, const float b,
                     const float a = 1);
  void SetDepthTest(const bool enable);
  void Clear(const unsigned int buffer_type = GL_COLOR_BUFFER_BIT);
  void SetViewport(const unsigned int width, const unsigned int height,
                   const unsigned int left_offset = 0,
                   const unsigned int bottom_offset = 0);

  std::string GetStringOpenGL(const unsigned int name);
};
}  // namespace RenderEngine

#endif  // RENDERER_H