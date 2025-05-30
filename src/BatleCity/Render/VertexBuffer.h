#pragma once
#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <glad/glad.h>

#include <cstddef>

namespace RenderEngine {
class VertexBuffer {
 public:
  VertexBuffer();
  ~VertexBuffer();

  VertexBuffer(const VertexBuffer&) = delete;
  VertexBuffer& operator=(const VertexBuffer&) = delete;

  VertexBuffer(VertexBuffer&& outher_vertex_buffer) noexcept;
  VertexBuffer& operator=(VertexBuffer&& outher_vertex_buffer) noexcept;

  void Init(const void* data, const size_t size);
  void Update(const void* data, const size_t size) const;
  void Bind() const;
  void Unbind() const;

 private:
  GLuint m_buffer_id = 0;
};
}  // namespace RenderEngine

#endif  // !VERTEX_BUFFER_H
