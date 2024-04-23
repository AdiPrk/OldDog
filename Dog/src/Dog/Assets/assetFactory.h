#pragma once

#include "asset.h"
#include "Dog/Logger/logger.h"

namespace Dog {

	class  AssetFactory {
	public:
		using CreatorFunc = std::function<std::shared_ptr<Asset>()>;

		template<typename T>
		void registerType(const std::string& typeName) {
			creators[typeName] = []() -> std::shared_ptr<Asset> {
				return std::make_shared<T>();
			};
		}

		std::shared_ptr<Asset> create(const std::string& typeName) {
			auto it = creators.find(typeName);
			if (it != creators.end()) {
				return it->second();
			}
			else {
				// also include file and line number that the log came from.
				DOG_INFO("*Sad Woof*");
				DOG_ASSERT(false, "Failed to find factory method for Type: {0}", typeName);
			}
			return nullptr;
		}

	private:
		std::unordered_map<std::string, CreatorFunc> creators;
	};

}