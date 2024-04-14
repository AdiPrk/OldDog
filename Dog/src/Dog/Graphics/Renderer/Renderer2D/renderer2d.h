#pragma once

#include "../IRenderer.h"
#include "Dog/Graphics/Renderer/Shaders/shader.h"

namespace Dog {

	class Texture2D;

	class Renderer2D : public IRenderer {
	public:
		virtual ~Renderer2D() override;

		virtual void initialize() override;
		virtual void shutdown() override;

		virtual void clearFrame() override;
		virtual void beginFrame() override;
		virtual void endFrame() override;

		void DrawSprite(const Texture2D& texture, const glm::mat4& transform, glm::vec4 color = glm::vec4(1.0f), glm::vec2 repetition = glm::vec2(0), float depth = 0) const;
		void DrawSprite(const Texture2D& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotation = 0.0f, glm::vec4 color = glm::vec4(1.0f), glm::vec2 repetition = glm::vec2(0), float depth = 0) const;

	private:
		unsigned int quadVAO = 0;
	};

} // namespace Dog
