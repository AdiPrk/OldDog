#pragma once

#include "Dog/core.h"
#include "Dog/Graphics/Renderer/Renderer2D/texture2d.h"

namespace Dog {

	struct DOG_API TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag)
		{
		}
	};

	struct DOG_API TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 100.0f, 100.0f, 100.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) 
		{
		}

		glm::mat4 GetTransform() const
		{
			// SRT - Translate * Rotate * Scale
			return glm::translate(glm::mat4(1.0f), Translation) * glm::toMat4(glm::quat(Rotation)) * glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct DOG_API SpriteComponent
	{
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		Texture2D Texture;

		SpriteComponent();
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent(const glm::vec4& color, const std::string& texturePath);
	};

}
