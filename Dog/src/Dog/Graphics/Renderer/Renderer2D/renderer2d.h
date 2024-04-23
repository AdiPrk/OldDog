#pragma once

#include "../IRenderer.h"

namespace Dog {

	class Texture2D;
	class Shader;

	class Renderer2D : public IRenderer {
	public:
		virtual ~Renderer2D() override;

		virtual void initialize() override;
		virtual void shutdown() override;

		virtual void clearFrame() override;
		virtual void beginFrame() override;
		virtual void endFrame() override;

		void DrawSprite(std::weak_ptr<Texture2D> texture, std::weak_ptr<Shader> shader, const glm::mat4& transform, glm::vec4 color = glm::vec4(1.0f), glm::vec2 repetition = glm::vec2(0), float depth = 0);

	private:
		unsigned int quadVAO = 0;

		unsigned activeShaderID = 0;
	};

} // namespace Dog
