#include <PCH/dogpch.h>
#include "resources.h"
#include "resourceFactory.h"
#include "resource.h"
#include "Dog/Logger/logger.h"
#include "Dog/Graphics/Shaders/shader.h"
#include "Dog/Graphics/Renderer/Renderer2D/texture2d.h"

namespace Dog {

	Resources::Resources()
		: factory(new ResourceFactory)
	{
		factory->registerType<Shader>("Shader");
		factory->registerType<Texture2D>("Texture2D");
	}

	Resources::~Resources()
	{
		UnloadAll();
	}

	std::shared_ptr<Resource> Resources::Load(const std::string& type, const std::string& filePath)
	{
		DOG_CORE_TRACE("Loading resource: {}", "'" + type + "' from " + filePath);

		auto resource = factory->create(type);
		if (resource) {
			resource->load(filePath);
			resources[filePath] = resource;
		}
		else {
			// also include file and line number that the log came from.
			DOG_CORE_CRITICAL("Failed to load resource: {}", filePath);
			DOG_CORE_CRITICAL("From file: {}", __FILE__);
			DOG_CORE_CRITICAL("From line: {}", __LINE__);
			DOG_CORE_INFO("*Sad Woof*");
		}
		return resource;
	}

	void Resources::Unload(const std::string& filePath)
	{
		resources.erase(filePath);
	}

	void Resources::UnloadAll()
	{
		resources.clear();
	}

}
