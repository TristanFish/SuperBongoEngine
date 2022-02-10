#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <vector>

#include "Colour.h"
#include "glew/glew.h"

struct BufferTexture
{
	enum class TexType
	{
		FOUR_COMP_SIGNED_COLOUR,
		THREE_COMP_SIGNED_COLOUR,
		ONE_COMP_SIGNED_COLOUR,
		ONE_COMP_UNSIGNED_SHORT,
		ONE_COMP_SIGNED_INT

	};

	GLuint texture;
	TexType type;

	BufferTexture() = default;
	
	BufferTexture(TexType type_)
	{
		texture = 0;
		type = type_;
	}

	GLuint GetTexture() const { return texture; }

	void DeleteTexture();
	
};


struct FrameBuffer
{
	
	GLuint bufferID;
	std::vector<BufferTexture> attachedTextures;
	Colour clearColor;

	FrameBuffer() = default;

	void InitFrameBuffer();
	void AttachTexture(BufferTexture& tex);
	void FinalizeBuffer() const;

	GLuint GetFrameBuffer() const { return bufferID; }
	void Bind() const { glBindFramebuffer(GL_FRAMEBUFFER, bufferID); }
	void UnBind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
	void Clear();

	void DeleteFramebuffer();

	int ReadPixel(uint32_t attachmentIndex, int x, int y);
	
};

#endif