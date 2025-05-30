#pragma once
#ifndef VERTEX_BUFFER_LAYOUT_H
#define VERTEX_BUFFER_LAYOUT_H

#include <glad/glad.h>
#include <vector>

namespace RenderEngine {
struct VertexBufferLayoutElement {
  GLuint count;
  GLuint type;
  GLboolean normalized;
  unsigned int size;
};

class VertexBufferLayout {
 public:
  VertexBufferLayout() = default;
  ~VertexBufferLayout() = default;

  inline GLuint GetStride() const { return m_stride; }
  void Reserve(const std::size_t count);
  void AddBufferLayoutElementFloat(const GLuint count,
                                   const GLboolean normalized);
  const std::vector<VertexBufferLayoutElement>& GetLayoutElements() const;

 private:
  std::vector<VertexBufferLayoutElement> m_layoutElementsOfBuffer;
  GLuint m_stride = 0;
};
}  // namespace RenderEngine

#endif  // VERTEX_BUFFER_LAYOUT_H