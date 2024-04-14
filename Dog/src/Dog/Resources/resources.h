#pragma once


#include "Dog/Resources/resource.h"

namespace Dog {
	
	class ResourceFactory;

	class Resources {
	public:
		Resources();
		~Resources();

		static Resources& Get() {
			static Resources instance;
			return instance;
		}

		Resources(const Resources&) = delete;
		Resources& operator=(const Resources&) = delete;

		std::shared_ptr<Resource> Load(const std::string& type, const std::string& filePath);

		// templated Load
		template <typename T>
		std::shared_ptr<T> Load(const std::string& filePath) {
			// static assert if T::GetTypeName() isn't existant
			static_assert(std::is_base_of<Resource, T>::value, "T must derive from Resource");

			std::string type = T::GetTypeName();
			auto resource = Load(type, filePath);
			return std::dynamic_pointer_cast<T>(resource);
		}

		// Get resource by filePath
		template <typename T>
		std::shared_ptr<T> Get(const std::string& filePath) {
			// static assert if T::GetTypeName() isn't existant
			static_assert(std::is_base_of<Resource, T>::value, "T must derive from Resource");

			auto it = resources.find(filePath);
			if (it != resources.end()) {
				auto resource = it->second;
				return std::dynamic_pointer_cast<T>(resource);
			}
			return nullptr;
		}
		
		void Unload(const std::string& filePath);
		void UnloadAll();

	private:
		std::unordered_map<std::string, std::shared_ptr<Resource>> resources;
		ResourceFactory* factory;
	};

}
