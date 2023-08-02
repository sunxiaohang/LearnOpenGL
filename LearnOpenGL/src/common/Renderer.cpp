#include "Renderer.h"

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	va.Bind();
	ib.Bind();
	shader.Bind();
	GLTryCall(glDrawElements(GL_TRIANGLES, sizeof(ib), GL_UNSIGNED_INT, NULL));// draw our first triangle
}

void Renderer::Clear() const
{
	GLTryCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLTryCall(glClear(GL_COLOR_BUFFER_BIT));
}