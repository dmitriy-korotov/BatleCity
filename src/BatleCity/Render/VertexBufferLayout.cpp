#include <BatleCity/Render/VertexBufferLayout.h>

namespace RenderEngine {
void VertexBufferLayout::Reserve(const std::size_t count) {
  m_layoutElementsOfBuffer.reserve(count);
}

void VertexBufferLayout::AddBufferLayoutElementFloat(
    const GLuint count, const GLboolean normalized) {
  m_layoutElementsOfBuffer.push_back(
      {count, GL_FLOAT, normalized,
       static_cast<unsigned int>(count * sizeof(GLfloat))});
  m_stride += m_layoutElementsOfBuffer.back().count * sizeof(GLfloat);
}

const std::vector<VertexBufferLayoutElement>&
VertexBufferLayout::GetLayoutElements() const {
  return m_layoutElementsOfBuffer;
}
}  // namespace RenderEngine