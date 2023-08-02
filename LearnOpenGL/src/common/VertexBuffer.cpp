#include "VertexBuffer.h"
#include "Utils.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	GLTryCall(glGenBuffers(1, &m_RendererID));
	GLTryCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLTryCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLTryCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
	GLTryCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::UnBind() const
{
	GLTryCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
