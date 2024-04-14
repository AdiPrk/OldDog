#include <PCH/dogpch.h>
#include "components.h"
#include "Dog/engine.h"

namespace Dog {

	SpriteComponent::SpriteComponent()
		: Color(1.0f)
		, texturePath("DogAssets/Images/square.png")
	{
	}

	SpriteComponent::SpriteComponent(const glm::vec4& color, const std::string& texturePath)
		: Color(color)
		, texturePath(texturePath)
	{
	}

} // namespace Dog
