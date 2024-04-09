#pragma once

#include "Dog/core.h"
#include "resource.h"

namespace Dog {

	class  ResourceFactory {
	public:
		using CreatorFunc = std::function<std::shared_ptr<Resource>()>;

		template<typename T>
		void registerType(const std::string& typeName) {
			creators[typeName] = []() -> std::shared_ptr<Resource> {
				return std::make_shared<T>();
				};
		}

		std::shared_ptr<Resource> create(const std::string& typeName) {
			auto it = creators.find(typeName);
			if (it != creators.end()) {
				return it->second();
			}
			return nullptr;
		}

	private:
		std::unordered_map<std::string, CreatorFunc> creators;
	};

}