#include <PCH/dogpch.h>
#include "components.h"
#include "Dog/engine.h"
#include "Dog/Graphics/Texture/texture2d.h"
#include "Dog/Graphics/Renderer/Camera/orthoCamera.h"
#include "Dog/Graphics/Renderer/Camera/perspectiveCamera.h"
#include "Dog/Graphics/Window/Iwindow.h"

namespace Dog {

	SpriteComponent::SpriteComponent()
		: Color(1.0f)
		, texturePath("square.png")
	{
	}

	SpriteComponent::SpriteComponent(const glm::vec4& color, const std::string& texturePath)
		: Color(color)
		, texturePath(texturePath)
	{
	}

	void CameraComponent::SetAsCurrentCamera()
	{
		//
	}

	CameraComponent::CameraComponent()
	{
		float width = (float)Engine::Get().GetWindow()->GetWidth();
		float height = (float)Engine::Get().GetWindow()->GetHeight();
		float aspectRatio = width / height;

		orthoCamera = std::make_unique<OrthographicCamera>(aspectRatio, OrthographicSize);
		//perspCamera = std::make_unique<PerspectiveCamera>();
	}

	CameraComponent::CameraComponent(const CameraComponent& other)
		: Projection(other.Projection)
		, OrthographicSize(other.OrthographicSize)
		, OrthographicNear(other.OrthographicNear)
		, OrthographicFar(other.OrthographicFar)
		, PerspectiveFOV(other.PerspectiveFOV)
		, PerspectiveNear(other.PerspectiveNear)
		, PerspectiveFar(other.PerspectiveFar)
		, orthoCamera(std::make_unique<OrthographicCamera>(*other.orthoCamera))
		//, perspCamera(std::make_unique<PerspectiveCamera>(*other.perspCamera))
	{
	}

	CameraComponent::CameraComponent(CameraType type)
		: Projection(type)
	{
		float width = (float)Engine::Get().GetWindow()->GetWidth();
		float height = (float)Engine::Get().GetWindow()->GetHeight();
		float aspectRatio = width / height;

		orthoCamera = std::make_unique<OrthographicCamera>(aspectRatio, OrthographicSize);
		//perspCamera = std::make_unique<PerspectiveCamera>();
	}

	void CameraComponent::UpdateCamera()
	{

	}

	ShaderComponent::ShaderComponent()
		: shaderPath("defaultsprite")
	{
	}

	ShaderComponent::ShaderComponent(const ShaderComponent& other)
		: shaderPath(other.shaderPath)
	{
	}

	ShaderComponent::ShaderComponent(const std::string& shaderPath) 
		: shaderPath(shaderPath) 
	{
	}

} // namespace Dog
