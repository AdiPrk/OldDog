#include <PCH/dogpch.h>
#include "components.h"
#include "Dog/Resources/resources.h"
#include "Dog/engine.h"

namespace Dog {

	SpriteComponent::SpriteComponent()
		: Color(1.0f)
		, Texture(Engine::Get().GetResources()->Get<Texture2D>("DogAssets/Images/square.png"))
	{
	}

	SpriteComponent::SpriteComponent(const glm::vec4& color, const std::string& texturePath)
		: Color(color)
		, Texture(Engine::Get().GetResources()->Get<Texture2D>(texturePath))
	{
	}

} // namespace Dog
