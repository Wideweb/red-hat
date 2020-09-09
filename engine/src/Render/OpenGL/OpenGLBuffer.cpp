#include "OpenGLBuffer.hpp"
#include "Debug.hpp"
#include <cassert>

namespace Engine {

/////////////////////////////////////////////////////////////////////////////
// VertexBuffer /////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) {
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(float *data, uint32_t size) {
    glGenBuffers(1, &m_RendererID);
    GL_CHECK();
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    GL_CHECK();
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    GL_CHECK();
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() { glDeleteBuffers(1, &m_RendererID); }

void OpenGLVertexBuffer::setData(void *data, uint32_t size) {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    GL_CHECK();
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    GL_CHECK();
}

void OpenGLVertexBuffer::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    GL_CHECK();
}

void OpenGLVertexBuffer::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    GL_CHECK();
}

void OpenGLVertexBuffer::setLayout(const BufferLayout &layout) {
    m_Layout = layout;
}

////////////////////////////////////////////////////////////////////////////
// IndexBuffer /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

OpenGLIndexBuffer::OpenGLIndexBuffer(const std::vector<uint32_t> &indexes)
    : m_Indexes(indexes), m_Count(indexes.size()) {
    glGenBuffers(1, &m_RendererID);
    GL_CHECK();
}
OpenGLIndexBuffer::~OpenGLIndexBuffer() { glDeleteBuffers(1, &m_RendererID); }

void OpenGLIndexBuffer::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    GL_CHECK();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indexes.size() * sizeof(uint32_t),
                 m_Indexes.data(), GL_STATIC_DRAW);
    GL_CHECK();
}

void OpenGLIndexBuffer::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    GL_CHECK();
}

} // namespace Engine
