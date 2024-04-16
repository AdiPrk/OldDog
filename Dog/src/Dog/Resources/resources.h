#pragma once

#include "Dog/Resources/resource.h"

namespace Dog {
	
	class ResourceFactory;

	class Resources {
	public:
		static void Init();
		static void Shutdown();

		static std::shared_ptr<Resource> Load(const std::string& type, const std::string& filePath);

		// templated Load
		template <typename T>
		static std::shared_ptr<T> Load(const std::string& filePath) {
			// static assert if T::GetTypeName() isn't existant
			static_assert(std::is_base_of<Resource, T>::value, "T must derive from Resource");

			std::string type = T::GetTypeName();
			auto resource = Load(type, filePath);
			return std::dynamic_pointer_cast<T>(resource);
		}

		// Get resource by filePath
		// Requires full relative path
		template <typename T>
		static std::shared_ptr<T> Get(const std::string& filePath) {
			// static assert if T doesn't derive from resource
			static_assert(std::is_base_of<Resource, T>::value, "T must derive from Resource");

			auto it = resources.find(filePath);
			if (it != resources.end()) {
				auto resource = it->second;
				return std::dynamic_pointer_cast<T>(resource);
			}

			DOG_ERROR("Resoure {0} not found in Resources", filePath);

			return nullptr;
		}

		// Convenience functions for getting resources
		template <typename T>
		static std::shared_ptr<T> GetImage(const std::string& filePath) {
			return Get<T>(ImagesPath + filePath);
		}

		template <typename T>
		static std::shared_ptr<T> GetShader(const std::string& filePath) {
			return Get<T>(ShadersPath + filePath);
		}
		
		static void Unload(const std::string& type, const std::string& filePath);
		static void UnloadAll();

		inline static const std::string AssetsDir = "DogAssets/";

		inline static const std::string EditorDir = "Editor/";
		inline static const std::string ShadersDir = "Shaders/";
		inline static const std::string ImagesDir = "Images/";

		inline static const std::string EditorPath = AssetsDir + EditorDir;
		inline static const std::string ShadersPath = AssetsDir + ShadersDir;
		inline static const std::string ImagesPath = AssetsDir + ImagesDir;

	private:
		static std::unordered_map<std::string, std::shared_ptr<Resource>> resources;
		static ResourceFactory* factory;
		static std::mutex resourcesMutex;

		friend class Engine;
		enum class FileAction {
			Create,
			Delete,
			Modify
		};
		static std::vector<std::pair<FileAction, std::string>> imageFileActions;
		static std::vector<std::pair<FileAction, std::string>> shaderFileActions;

		static void UpdateResources();
		static void OnImageFileCreate(const Event::ImageFileCreated& event);
		static void OnImageFileDelete(const Event::ImageFileDeleted& event);
		static void OnImageFileModify(const Event::ImageFileModified& event);
		static void OnShaderFileCreate(const Event::ShaderFileCreated& event);
		static void OnShaderFileDelete(const Event::ShaderFileDeleted& event);
		static void OnShaderFileModify(const Event::ShaderFileModified& event);

		std::mutex fileActionsMutex;

	};

}
