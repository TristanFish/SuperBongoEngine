#include "FrameBuffer.h"

#include "core/Globals.h"
#include "core/Logger.h"

void BufferTexture::DeleteTexture()
{
	glDeleteTextures(1, &texture);
}

void FrameBuffer::InitFrameBuffer()
{
	glGenFramebuffers(1, &bufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, bufferID);
}

void FrameBuffer::AttachTexture(BufferTexture& tex)
{
	uint32_t intFormat;
	uint32_t format;
	uint32_t type;
	
	switch (tex.type)
	{
	case BufferTexture::TexType::FOUR_COMP_SIGNED_COLOUR:
		intFormat = GL_RGBA16F;
		format = GL_RGBA;
		type = GL_UNSIGNED_BYTE;
		break;
	case BufferTexture::TexType::THREE_COMP_SIGNED_COLOUR:
		intFormat = GL_RGB16F;
		format = GL_RGBA;
		type = GL_FLOAT;
		break;
	case BufferTexture::TexType::ONE_COMP_SIGNED_COLOUR:
		intFormat = GL_R16F;
		format = GL_RED;
		type = GL_FLOAT;
		break;
	case BufferTexture::TexType::ONE_COMP_UNSIGNED_SHORT:
		intFormat = GL_R16UI;
		format = GL_RED_INTEGER;
		type = GL_UNSIGNED_SHORT;
		break;
	case BufferTexture::TexType::ONE_COMP_UNSIGNED_INT:
		intFormat = GL_R32UI;
		format = GL_RED_INTEGER;
		type = GL_UNSIGNED_INT;
		break;
	default:
		EngineLogger::Error("FrameBuffer TextureType not recognized", "FrameBuffer.cpp", __LINE__, MessageTag::TYPE_GRAPHICS);
		return;
	}

	const uint32_t texNum = +GL_COLOR_ATTACHMENT0 + attachedTextures.size();

	if(texNum > 15 + GL_COLOR_ATTACHMENT0)
	{
		EngineLogger::Error("FrameBuffer has too many textures", "FrameBuffer.cpp", __LINE__, MessageTag::TYPE_GRAPHICS);
	}

	MATH::Vec2 ViewportSize = Renderer::GetInstance()->GetViewport().GetViewportSize();

	glGenTextures(1, &tex.texture);
	glBindTexture(GL_TEXTURE_2D, tex.texture);
	glTexImage2D(GL_TEXTURE_2D, 0, intFormat, ViewportSize.x, ViewportSize.y, 0, format, type, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, bufferID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, texNum, GL_TEXTURE_2D, tex.texture, 0);

	attachedTextures.push_back(tex);
}

void FrameBuffer::FinalizeBuffer() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, bufferID);
	
	GLenum ca[15];
	for(size_t i = 0; i < attachedTextures.size(); i++)
	{
		ca[i] = i + GL_COLOR_ATTACHMENT0;
	//	colorAttachments.push_back(i + GL_COLOR_ATTACHMENT0);
	}

	
	
	glDrawBuffers(attachedTextures.size(), ca);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		EngineLogger::Error("FrameBuffer not complete", "Renderer.cpp", __LINE__, MessageTag::TYPE_GRAPHICS);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Clear()
{
	MATH::Vec2 ViewportSize = Renderer::GetInstance()->GetViewport().GetViewportSize();

	glViewport(0, 0, ViewportSize.x, ViewportSize.y);
	glClearColor(clearColor.a, clearColor.g, clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void FrameBuffer::DeleteFramebuffer()
{
	for (BufferTexture& tex : attachedTextures)
	{
		tex.DeleteTexture();
	}

	glDeleteFramebuffers(1, &bufferID);

	attachedTextures.clear();
}

uint32_t FrameBuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
{
	Bind();
	if (attachmentIndex < attachedTextures.size())
	{
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		uint32_t pix_value = 0; 
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &pix_value);
		
		return pix_value;

	}
	UnBind();
}
