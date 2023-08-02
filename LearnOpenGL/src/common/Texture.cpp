#include "Texture.h"
#include "std_image/stb_image.h"

Texture::Texture(const std::string& filePath)
	:m_RendererID(0),m_FilePath(filePath), m_LocalBuffer(nullptr),m_Width(0),m_Height(0),m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_BPP,4);
	GLTryCall(glGenTextures(1, &m_RendererID));
	GLTryCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	GLTryCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLTryCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLTryCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLTryCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLTryCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLTryCall(glBindTexture(GL_TEXTURE_2D, 0));
	if(m_LocalBuffer) stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
	GLTryCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	GLTryCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLTryCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::UnBind() const
{
	GLTryCall(glBindTexture(GL_TEXTURE_2D, 0));
}
