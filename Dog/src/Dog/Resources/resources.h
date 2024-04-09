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
		
		void Unload(const std::string& filePath);
		void UnloadAll();

	private:
		std::unordered_map<std::string, std::shared_ptr<Resource>> resources;
		ResourceFactory* factory;
	};

}
