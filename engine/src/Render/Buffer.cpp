#include "Buffer.hpp"
#include "OpenGLBuffer.hpp"

namespace Engine {

VertexBuffer *VertexBuffer::create(uint32_t size) {
    return new OpenGLVertexBuffer(size);
}

VertexBuffer *VertexBuffer::create(float *data, uint32_t size) {
    return new OpenGLVertexBuffer(data, size);
}

IndexBuffer *IndexBuffer::create(const std::vector<uint32_t> &indexes) {
    return new OpenGLIndexBuffer(indexes);
}

} // namespace Engine
