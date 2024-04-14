#include <PCH/dogpch.h>
#include "resources.h"
#include "resourceFactory.h"
#include "resource.h"
#include "Dog/Logger/logger.h"
#include "Dog/Graphics/Renderer/Shaders/shader.h"
#include "Dog/Graphics/Renderer/Renderer2D/texture2d.h"

#define REGISTER_TYPE(type) factory->registerType<type>(type::GetTypeName())

namespace Dog {

	std::unordered_map<std::string, std::shared_ptr<Resource>> Resources::resources;
	ResourceFactory* Resources::factory = nullptr;
	std::mutex Resources::resourcesMutex;
	std::vector<std::pair<Resources::FileAction, std::string>> Resources::imageFileActions;
	std::vector<std::pair<Resources::FileAction, std::string>> Resources::shaderFileActions;

	void Resources::Init() {
		factory = new ResourceFactory();

		REGISTER_TYPE(Shader);
		REGISTER_TYPE(Texture2D);
	}

	void Resources::Shutdown() {
		UnloadAll();
		delete factory;
	}

	std::shared_ptr<Resource> Resources::Load(const std::string& type, const std::string& filePath)
	{
		std::lock_guard<std::mutex> lock(resourcesMutex);

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
			if (resource->load(filePath)) {
				resources[filePath] = resource;
			}
			else {
				DOG_ERROR("Failed to load resource: {}", filePath);
				DOG_INFO("*Sad Woof*");
				return nullptr;
			}
		}
		else {
			// also include file and line number that the log came from.
			DOG_INFO("*Sad Woof*");
			DOG_ASSERT(false, "Failed to load resource: {}", filePath);
		}
		return resource;
	}

	void Resources::Unload(const std::string& type, const std::string& filePath)
	{
		std::lock_guard<std::mutex> lock(resourcesMutex);

		DOG_INFO("Unloading resource: {}", "'" + type + "' from " + filePath);

		auto it = resources.find(filePath);
		if (it != resources.end()) {
			resources.erase(it);
		}
		else {
			DOG_WARN("Warning: Unloading a non-loaded resource: {}", filePath);
		}
	}

	void Resources::UnloadAll()
	{
		std::lock_guard<std::mutex> lock(resourcesMutex);
		resources.clear();
	}

	void Resources::UpdateResources() {
		for (const auto& imageFileChange : imageFileActions) {
			switch (imageFileChange.first) {
			case FileAction::Create:
				Load("Texture2D", imageFileChange.second);
				break;
			case FileAction::Delete:
				Unload("Texture2D", imageFileChange.second);
				break;
			case FileAction::Modify:
				Unload("Texture2D", imageFileChange.second);
				Load("Texture2D", imageFileChange.second);
				break;
			default:
				break;
			}
		}

		imageFileActions.clear();

		for (const auto& shaderFileChange : shaderFileActions) {
			std::string fp = shaderFileChange.second;
			std::string result;
			
			// Find the last occurrence of '.' and cut it from there
			size_t pos = fp.rfind('.');
			if (pos != std::string::npos) {
				result = fp.substr(0, pos);
			}
			else {
				return;
			}

			switch (shaderFileChange.first) {
			case FileAction::Create:
				Load("Shader", result);
				break;
			case FileAction::Delete:
				Unload("Shader", result);
				break;
			case FileAction::Modify:
				Unload("Shader", result);
				if (!Load("Shader", result)) {
					break;
				}
				else {
					Shader::SetShader(Get<Shader>(result));
				}
				break;
			default:
				break;
			}
		}

		shaderFileActions.clear();
	}

	void Resources::OnImageFileCreate(const Event::ImageFileCreated& event)
	{
		// lock mutex
		std::lock_guard<std::mutex> lock(resourcesMutex);
		imageFileActions.push_back(std::make_pair(FileAction::Create, event.path));
	}

	void Resources::OnImageFileDelete(const Event::ImageFileDeleted& event)
	{
		std::lock_guard<std::mutex> lock(resourcesMutex);
		imageFileActions.push_back(std::make_pair(FileAction::Delete, event.path));
	}

	void Resources::OnImageFileModify(const Event::ImageFileModified& event)
	{
		std::lock_guard<std::mutex> lock(resourcesMutex);
		imageFileActions.push_back(std::make_pair(FileAction::Modify, event.path));
	}

	void Resources::OnShaderFileCreate(const Event::ShaderFileCreated& event)
	{
		std::lock_guard<std::mutex> lock(resourcesMutex);
		shaderFileActions.push_back(std::make_pair(FileAction::Create, event.path));
	}

	void Resources::OnShaderFileDelete(const Event::ShaderFileDeleted& event)
	{
		std::lock_guard<std::mutex> lock(resourcesMutex);
		shaderFileActions.push_back(std::make_pair(FileAction::Delete, event.path));
	}

	void Resources::OnShaderFileModify(const Event::ShaderFileModified& event)
	{
		std::lock_guard<std::mutex> lock(resourcesMutex);
		shaderFileActions.push_back(std::make_pair(FileAction::Modify, event.path));
	}

}
