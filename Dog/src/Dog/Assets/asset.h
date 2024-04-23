#pragma once

#include "UUID/uuid.h"

namespace Dog {

	class Asset {
	public:
		virtual ~Asset() {};

		static const std::string GetTypeName() { return "Asset"; }

		// Must be implemented by every asset.
		// Loads the asset from a given path.
		// Returns true if the asset was loaded successfully.
		virtual bool load(const std::string& path) = 0;
		virtual bool loadFromData(const std::vector<char>& data) = 0;

		// Not yet implemented.
		// To be done later.
		virtual void save(const std::string& path) {}

		UUID GetUUID() const { return uuid; }

	private:
		UUID uuid;
	};

}
