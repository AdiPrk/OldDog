#include <PCH/dogpch.h>
#include "resources.h"
#include "resourceFactory.h"
#include "resource.h"
#include "Dog/Logger/logger.h"
#include "Dog/Graphics/Renderer/Shaders/shader.h"
#include "Dog/Graphics/Renderer/Renderer2D/texture2d.h"

#define REGISTER_TYPE(type) factory->registerType<type>(type::GetTypeName())

namespace Dog {

	Resources::Resources()
		: factory(new ResourceFactory)
	{
		REGISTER_TYPE(Shader);
		REGISTER_TYPE(Texture2D);
	}

	Resources::~Resources()
	{
		UnloadAll();
	}

	std::shared_ptr<Resource> Resources::Load(const std::string& type, const std::string& filePath)
	{
		DOG_INFO("Loading resource: {}", "'" + type + "' from " + filePath);

		// first check if it already exists
		auto it = resources.find(filePath);
		if (it != resources.end()) {
			DOG_WARN("Warning: Loading an already loaded resource: {}", filePath);
			DOG_WARN("Returning the previously loaded resource.");
			return it->second;
		}

		auto resource = factory->create(type);
		if (resource) {
			resource->load(filePath);
			resources[filePath] = resource;
		}
		else {
			// also include file and line number that the log came from.
			DOG_INFO("*Sad Woof*");
			DOG_ASSERT(false, "Failed to load resource: {}", filePath);
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
