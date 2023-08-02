#pragma once
#include "Utils.h"

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height,m_BPP;
public:
	Texture(const std::string& filePath);
	~Texture();

	void Bind(unsigned int slot = 0)const;
	void UnBind()const;
	inline unsigned int getWidth() { return m_Width; }
	inline unsigned int getHeight() { return m_Height; }
};

