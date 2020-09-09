#pragma once

#include "Buffer.hpp"
#include "glad/glad.h"

namespace Engine {

class OpenGLVertexBuffer : public VertexBuffer {
  private:
    BufferLayout m_Layout;
    uint32_t m_RendererID;

  public:
    OpenGLVertexBuffer(uint32_t size);
    OpenGLVertexBuffer(float *data, uint32_t size);
    virtual ~OpenGLVertexBuffer();

    virtual void bind() override;
    virtual void unbind() override;

    virtual void setLayout(const BufferLayout &layout) override;
    virtual const BufferLayout &getLayout() const override { return m_Layout; }

    virtual void setData(const void *data, uint32_t size) override;
};

class OpenGLIndexBuffer : public IndexBuffer {
  private:
    std::vector<uint32_t> m_Indexes;
    uint32_t m_Count;
    uint32_t m_RendererID;

  public:
    OpenGLIndexBuffer(const std::vector<uint32_t> &indexes);
    virtual ~OpenGLIndexBuffer();

    virtual void bind() override;
    virtual void unbind() override;

    virtual int32_t getCount() const override { return m_Count; };
};

} // namespace Engine
