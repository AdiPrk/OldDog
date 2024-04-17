#pragma once

namespace Dog {

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag)
		{
		}
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

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

	struct SpriteComponent
	{
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		std::string texturePath;

		SpriteComponent();
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent(const glm::vec4& color, const std::string& texturePath);
	};

	struct ShaderComponent
	{
		std::string shaderPath;

		ShaderComponent() = default;
		ShaderComponent(const ShaderComponent&) = default;
		ShaderComponent(const std::string& shaderPath) : shaderPath(shaderPath) {}
	};

	class OrthographicCamera;
	//class PerspectiveCamera;

	struct CameraComponent
	{
		void SetAsCurrentCamera();

		enum class CameraType
		{
			Orthographic = 0,
			Perspective
		};

		bool MainCamera = false;

		CameraType Projection = CameraType::Orthographic;

		float OrthographicSize = 1.0f;
		float OrthographicNear = -1.0f;
		float OrthographicFar = 1.0f;

		float PerspectiveFOV = 45.0f;
		float PerspectiveNear = 0.01f;
		float PerspectiveFar = 100.0f;
		
		std::unique_ptr<OrthographicCamera> orthoCamera;
		//std::unique_ptr<PerspectiveCamera> perspCamera;

		CameraComponent();
		CameraComponent(const CameraComponent& other);
		CameraComponent(CameraType type);
		void UpdateCamera();
	};

}
