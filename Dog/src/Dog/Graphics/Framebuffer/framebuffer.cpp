#include <PCH/dogpch.h>
#include "framebuffer.h"
#include "Dog/Graphics/Renderer/Shaders/shader.h"

namespace Dog {

	FrameBuffer::FrameBuffer(const FrameBufferSpecification& spec)
		: specification(spec)
	{
		Create();
	}

	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &fbo);
		glDeleteTextures(1, &colorAttachment);
		glDeleteTextures(1, &depthAttachment);
	}

	void FrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glViewport(0, 0, specification.width, specification.height);
	}

	void FrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::OnResize(const Event::SceneResize& event)
	{
		if (event.width <= 0 || event.height <= 0)
			return;

		specification.width = event.width;
		specification.height = event.height;

		Shader::SetResolutionUBO(glm::vec2(event.width, event.height));

		DOG_INFO("FrameBuffer::OnResize({0}, {1})", specification.width, specification.height);

		Create();
	}

	void FrameBuffer::Create()
	{
		if (fbo)
		{
			glDeleteFramebuffers(1, &fbo);
			glDeleteTextures(1, &colorAttachment);
			glDeleteTextures(1, &depthAttachment);
		}

		glCreateFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		glCreateTextures(GL_TEXTURE_2D, 1, &colorAttachment);
		glBindTexture(GL_TEXTURE_2D, colorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, specification.width, specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &depthAttachment);
		glBindTexture(GL_TEXTURE_2D, depthAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, specification.width, specification.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthAttachment, 0);

		DOG_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

} // namespace Dog
