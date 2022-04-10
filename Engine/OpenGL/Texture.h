#pragma once
#include <cinttypes>
#include "glad/glad.h"
class Texture
{
public:
	Texture() = default;
	Texture(const char* path);
	void Create(uint32_t width, uint32_t height, unsigned char* data, GLenum format, GLenum dataType);
	void Create(const char* path);

	void Bind(uint32_t slot = 0) { glActiveTexture(GL_TEXTURE0 + slot); glBindTexture(GL_TEXTURE_2D, m_ID); }

	const uint32_t GetID() const { return m_ID; }
private:
	uint32_t m_Width, m_Height;
	uint32_t m_ID;
};