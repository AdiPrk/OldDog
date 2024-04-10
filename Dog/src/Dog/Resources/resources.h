#pragma once

#include "Dog/core.h"
#include "Dog/Resources/resource.h"

namespace Dog {
	
	class ResourceFactory;

	class DOG_API Resources {
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
		template<typename T>
		std::shared_ptr<T> Load(const std::string& filePath) {
			// static assert if T::GetTypeName() isn't existant
			static_assert(std::is_base_of<Resource, T>::value, "T must derive from Resource");

			std::string type = T::GetTypeName();
			auto resource = Load(type, filePath);
			return std::dynamic_pointer_cast<T>(resource);
		}
		
		void Unload(const std::string& filePath);
		void UnloadAll();

	private:
		std::unordered_map<std::string, std::shared_ptr<Resource>> resources;
		ResourceFactory* factory;
	};

}
