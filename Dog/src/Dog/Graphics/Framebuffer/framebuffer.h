#pragma once

namespace Dog {

	enum class FBAttachment {
		None = 0,
		RGBA = GL_RGBA,
		RGBA8 = GL_RGBA8,
		RGBA16F = GL_RGBA16F,
		RGBA32F = GL_RGBA32F,
		Depth24Stencil8 = GL_DEPTH24_STENCIL8
	};

	struct FrameBufferSpecification {
		int width, height;
		unsigned samples = 1;
		std::vector<FBAttachment> attachments;
	};

	class FrameBuffer {
	public:
		FrameBuffer(const FrameBufferSpecification& spec);
		~FrameBuffer();

		void Bind();
		void Unbind();

		unsigned GetColorAttachmentID(unsigned index) const {
			if (index >= colorAttachments.size()) {
				DOG_ASSERT(false, "Index out of bounds.");
				return 0;
			}
			return colorAttachments[index]; 
		}
		unsigned GetDepthAttachmentID() const { return depthAttachment; }


		const FrameBufferSpecification& GetSpecification() const { return specification; }


	private:
		void Create();
		void Clear();
		void AddColorAttachment(const FBAttachment& format);
		void AddDepthAttachment(const FBAttachment& format);

		// Only meant for the scene's FBO!
		friend class Scene;
		void OnSceneResize(const Event::SceneResize& event);

		unsigned fbo = 0;

		std::vector<unsigned> colorAttachments;
		unsigned depthAttachment = 0;

		FrameBufferSpecification specification;
	};

} // namespace Dog
