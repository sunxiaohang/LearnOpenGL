#include "IndexBuffer.h"
#include "Utils.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count):m_Count(count)
{
	GLTryCall(glGenBuffers(1, &m_RendererID));
	GLTryCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GLTryCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLTryCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind()const
{
	GLTryCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::UnBind()const
{
	GLTryCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
