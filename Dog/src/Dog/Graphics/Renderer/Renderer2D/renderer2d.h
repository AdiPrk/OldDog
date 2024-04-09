#pragma once

#include "Dog/core.h"
#include "../IRenderer.h"
#include "Dog/Graphics/Shaders/shader.h"

namespace Dog {

	class Texture2D;

	class DOG_API Renderer2D : public IRenderer {
	public:
		virtual ~Renderer2D() override;

		virtual void initialize() override;
		virtual void shutdown() override;

		virtual void beginFrame() override;
		virtual void endFrame() override;

		void DrawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotation = 0.0f, glm::vec4 color = glm::vec4(1.0f), glm::vec2 repetition = glm::vec2(0), float depth = 0);

		static void SetShader(Shader& shader) { activeShader = shader.Use(); }
		static Shader& GetActiveShader() { return activeShader; }
	private:
		unsigned int quadVAO = 0;
		static Shader activeShader;
	};

} // namespace Dog
