#include "VertexArray.h"

VertexArray::VertexArray()
{
	GLTryCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLTryCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLTryCall(glEnableVertexAttribArray(i));
		GLTryCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	GLTryCall(glBindVertexArray(m_RendererID));
}

void VertexArray::UnBind() const
{
	GLTryCall(glBindVertexArray(0));
}
