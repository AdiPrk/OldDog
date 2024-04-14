#pragma once

namespace Dog {

	struct FrameBufferSpecification {
		int width, height;
		unsigned samples = 1;

		bool SwapChainTarget = false;
	};

	class FrameBuffer {
	public:
		FrameBuffer(const FrameBufferSpecification& spec);
		~FrameBuffer();

		void Bind();
		void Unbind();

		void OnResize(const Event::SceneResize& event);

		unsigned GetColorAttachmentID() const { return colorAttachment; }
		unsigned GetDepthAttachmentID() const { return depthAttachment; }

		const FrameBufferSpecification& GetSpecification() const { return specification; }

	private:
		void Create();

		unsigned fbo = 0;
		unsigned colorAttachment = 0;
		unsigned depthAttachment = 0;
		FrameBufferSpecification specification;
	};

} // namespace Dog
