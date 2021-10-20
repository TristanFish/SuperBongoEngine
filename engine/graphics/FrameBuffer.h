#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <vector>
#include "glew/glew.h"

struct BufferTexture
{
	enum class TexType
	{
		FOUR_COMP_SIGNED_COLOUR,
		THREE_COMP_SIGNED_COLOUR,
		ONE_COMP_SIGNED_COLOUR,
		ONE_COMP_UNSIGNED_INT
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

	FrameBuffer() = default;

	void InitFrameBuffer();
	void AttachTexture(BufferTexture& tex);
	void FinalizeBuffer() const;

	GLuint GetFrameBuffer() const { return bufferID; }
	void Bind() const { glBindFramebuffer(GL_DRAW_FRAMEBUFFER, bufferID); }
	void Clear()
	{
		Bind();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void DeleteFramebuffer();
	
};

#endif